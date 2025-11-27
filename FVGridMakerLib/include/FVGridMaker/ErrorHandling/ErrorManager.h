// ----------------------------------------------------------------------------
// File: ErrorManager.h
// Project: FVGridMaker
// Version: 1.0
// Description: Define a fachada global ErrorManager e a implementação padrão
//              do logger (ThreadLocalBufferLogger). Resolve a dependência
//              circular na construção do ErrorConfig.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/ErrorConfig.h>

/**
 * @file ErrorManager.h
 * @brief Fachada de gerenciamento de erros e Logger padrão.
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @class ThreadLocalBufferLogger
 * @brief Implementação de Logger que armazena erros em memória local da thread.
 *
 * Esta estratégia evita a necessidade de locks (mutexes) para escrita, pois
 * cada thread possui seu próprio vetor estático. Ideal para alta performance
 * em paralelo, desde que cada thread seja responsável por fazer o flush de seus erros.
 */
class ThreadLocalBufferLogger : public IErrorLogger {
public:
    /**
     * @brief Armazena o registro no buffer da thread atual.
     * Respeita a capacidade definida em `ErrorConfig::thread_buffer_cap`.
     */
    void log(const ErrorRecord& record) override {
        auto cfg = Config::get();
        if (!cfg) return;

        auto cap = cfg->thread_buffer_cap;
        auto& buf = buffer();
        
        if (buf.size() < cap) {
            buf.push_back(record);
        }
        // Se cheio, o erro é descartado silenciosamente nesta implementação padrão.
    }

    /**
     * @brief Retorna e limpa todos os erros acumulados na thread atual.
     * @return std::vector<ErrorRecord> Lista de erros.
     */
    std::vector<ErrorRecord> flush() override {
        auto& buf = buffer();
        std::vector<ErrorRecord> out;
        // Swap é O(1), muito eficiente para "zerar" o buffer e pegar os dados
        out.swap(buf); 
        return out;
    }

private:
    /**
     * @brief Acesso ao armazenamento static thread_local.
     * Garante isolamento total entre threads.
     */
    static std::vector<ErrorRecord>& buffer() {
        static thread_local std::vector<ErrorRecord> tl_buf;
        return tl_buf;
    }
};

/**
 * @brief Construtor de ErrorConfig (Inline).
 * * Implementado aqui (e não em ErrorConfig.h) porque depende da definição
 * completa de `ThreadLocalBufferLogger`. Inicializa o logger padrão.
 */
inline ErrorConfig::ErrorConfig()
    : logger(std::make_shared<ThreadLocalBufferLogger>()) {}

/**
 * @class ErrorManager
 * @brief Fachada (Facade) estática para acesso ao sistema de logs.
 *
 * Atua como intermediário entre as macros de erro e o Logger configurado
 * no Singleton `Config`. Garante que chamadas de log sejam seguras mesmo
 * se a configuração estiver sendo trocada.
 */
class ErrorManager {
public:
    /**
     * @brief Envia um registro de erro para o logger ativo.
     * @param rec O registro de erro preenchido.
     */
    static void log(ErrorRecord rec) {
        if (auto cfg = Config::get()) {
            if (cfg->logger) {
                cfg->logger->log(std::move(rec));
            }
        }
    }

    /**
     * @brief Solicita a limpeza do buffer do logger ativo.
     * @return Vetor contendo os erros que estavam pendentes.
     */
    static std::vector<ErrorRecord> flush() {
        if (auto cfg = Config::get()) {
            if (cfg->logger) {
                return cfg->logger->flush();
            }
        }
        return {};
    }
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE