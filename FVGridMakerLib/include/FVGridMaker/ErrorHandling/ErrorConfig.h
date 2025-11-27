// ----------------------------------------------------------------------------
// File: ErrorConfig.h
// Project: FVGridMaker
// Version: 1.1
// Description: Define a estrutura de configuração de runtime (ErrorConfig) e
//              o Singleton thread-safe (Config) para gerenciar o estado global
//              do sistema de tratamento de erros.
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#pragma once


// ----------------------------------------------------------------------------
// includes c++
// ----------------------------------------------------------------------------
#include <memory>


// ----------------------------------------------------------------------------
// includes FVGridMaker
// ----------------------------------------------------------------------------
#include <FVGridMaker/ErrorHandling/Language.h>
#include <FVGridMaker/ErrorHandling/IErrorLogger.h>

/**
 * @file ErrorConfig.h
 * @brief Configurações globais de comportamento de erro (Runtime).
 *
 * Define como o sistema deve reagir a erros (lançar exceção vs retornar status),
 * qual idioma utilizar nas mensagens e qual logger utilizar.
 *
 * @ingroup ErrorHandling
 */

FVGRIDMAKER_NAMESPACE_OPEN
ERROR_NAMESPACE_OPEN

/**
 * @enum Policy
 * @brief Define a política de reação a erros.
 */
enum class Policy {
    /**
     * @brief Lança exceções (FVGException) para erros com severidade >= Error.
     * Ideal para aplicações C++ tradicionais que utilizam try-catch.
     */
    Throw,

    /**
     * @brief Não lança exceções. Apenas loga o erro.
     * Ideal para ambientes que desabilitam exceções ou usam o padrão Status/StatusOr.
     */
    Status
};

/**
 * @struct ErrorConfig
 * @brief Agrupamento de parâmetros de configuração do sistema de erros.
 *
 * Esta estrutura é imutável quando acessada via `Config::get()`. Para alterar
 * a configuração, cria-se uma nova instância desta struct e passa-se para `Config::set()`.
 */
struct ErrorConfig {
    /// @brief Idioma das mensagens de erro (Padrão: PtBR).
    Language language{Language::PtBR};

    /// @brief Política de tratamento de erros (Padrão: Throw).
    Policy policy{Policy::Throw};

    /// @brief Nível mínimo de severidade para logar (Padrão: Warning).
    Severity min_severity{Severity::Warning};

    /// @brief Capacidade máxima do buffer de erros thread-local (Padrão: 256).
    std::size_t thread_buffer_cap{256};

    /// @brief Ponteiro para o logger ativo (Padrão: ThreadLocalBufferLogger).
    std::shared_ptr<IErrorLogger> logger;

    /**
     * @brief Construtor padrão.
     * @note A implementação deste construtor reside em `ErrorManager.h` para
     * evitar dependências circulares na criação do logger padrão.
     */
    ErrorConfig();
};

/**
 * @class Config
 * @brief Singleton thread-safe para acesso à configuração global.
 *
 * Utiliza o padrão de "Snapshot" via `std::shared_ptr`. Leitores obtêm uma
 * referência segura para a configuração atual, enquanto escritores substituem
 * o ponteiro atomicamente (protegido por mutex).
 */
class Config {
public:
    /**
     * @brief Obtém um snapshot da configuração atual.
     *
     * Esta operação é thread-safe. O ponteiro retornado garante que a configuração
     * permanecerá válida para o chamador mesmo que outra thread chame `set()`
     * simultaneamente.
     *
     * @return std::shared_ptr<const ErrorConfig> Ponteiro somente leitura para a config.
     */
    static std::shared_ptr<const ErrorConfig> get() noexcept {
        std::lock_guard<std::mutex> lock(instance().mtx_);
        return instance().ptr_;
    }

    /**
     * @brief Define uma nova configuração global.
     *
     * Substitui a configuração ativa. Threads que já possuem um ponteiro obtido
     * via `get()` continuarão vendo a configuração antiga até soltarem a referência.
     *
     * @param cfg A nova estrutura de configuração.
     */
    static void set(ErrorConfig cfg) {
        // Cria o shared_ptr fora da seção crítica para performance
        auto sp = std::make_shared<const ErrorConfig>(std::move(cfg));
        std::lock_guard<std::mutex> lock(instance().mtx_);
        instance().ptr_ = sp;
    }

private:
    /**
     * @brief Construtor privado (Singleton).
     * Inicializa com uma configuração padrão.
     */
    Config() : ptr_(std::make_shared<const ErrorConfig>()) {}

    /**
     * @brief Acesso à instância única estática (Meyers Singleton).
     */
    static Config& instance() {
        static Config inst;
        return inst;
    }

    std::shared_ptr<const ErrorConfig> ptr_; ///< Ponteiro para a config ativa.
    std::mutex mtx_;                         ///< Mutex para proteger a troca do ponteiro.
};

ERROR_NAMESPACE_CLOSE
FVGRIDMAKER_NAMESPACE_CLOSE