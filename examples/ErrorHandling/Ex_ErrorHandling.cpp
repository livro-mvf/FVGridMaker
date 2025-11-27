// ----------------------------------------------------------------------------
// File: Ex_ErrorHandling_Full.cpp
// Project: FVGridMaker
// Version: 1.2
// Description: Exemplo COMPLETO do sistema de erros.
//              Demonstra:
//              1. Macros e Exceções (FVG_ERROR).
//              2. Padrão Funcional (Status / StatusOr).
//              3. Domínios de Erro customizados (GridErrors).
//              4. Injeção de Logger Customizado (IErrorLogger).
// Last modified: 2025-11-25
// Author: FVGridMaker Team
// License: GNU GPL v3
// ----------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Inclui o cabeçalho principal e os extras necessários
#include <FVGridMaker/ErrorHandling/ErrorHandling.h>
#include <FVGridMaker/ErrorHandling/Status.h>     // Para Status/StatusOr
#include <FVGridMaker/ErrorHandling/GridErrors.h> // Para erros de Malha

using namespace FVGridMaker::error;

// ============================================================================
// PART 1: Helpers para Demonstração
// ============================================================================

// Helper simples para criar um Status de erro a partir de um Enum
// (Em produção, isso poderia ser uma função utilitária na lib)
template <ErrorEnum E>
Status MakeErrorStatus(E code, std::string_view extra_msg = "") {
    ErrorRecord rec;
    rec.code = FVGridMaker::error::code(code);
    rec.severity = ErrorTraits<E>::default_severity(code);
    
    // Pega a mensagem traduzida bruta (sem placeholders complexos aqui para simplificar)
    // Para suporte completo, usaríamos a lógica do Detail::log_error
    auto cfg = Config::get();
    std::string_view tmpl = (cfg->language == Language::PtBR) 
                          ? ErrorTraits<E>::ptBR(code) 
                          : ErrorTraits<E>::enUS(code);
    
    rec.message = std::string(tmpl) + (extra_msg.empty() ? "" : " [Details: " + std::string(extra_msg) + "]");
    return Status(std::move(rec));
}

// Logger Customizado que imprime direto no Console (sem buffer)
class ConsoleLogger : public IErrorLogger {
public:
    void log(const ErrorRecord& rec) override {
        // Simplesmente imprime na tela imediatamente
        std::cerr << ">>> [CUSTOM LOG] " << rec.message 
                  << " (Code: " << rec.code << ")\n";
    }
};

// ============================================================================
// PART 2: Funções de Exemplo (Business Logic)
// ============================================================================

// Exemplo A: Função Clássica (Lança Exceção em erro)
void gerarMalha(int n) {
    if (n <= 0) {
        // Macro padrão: Loga e Lança (se Policy::Throw)
        FVG_ERROR(GridErr::InvalidN, {{"N", std::to_string(n)}});
    }
    std::cout << "   -> Malha gerada com " << n << " volumes.\n";
}

// Exemplo B: Função Funcional (Retorna Status, SEM Exceção)
// Ideal para loops internos onde try-catch seria lento.
Status validarGeometria(double area) {
    if (area <= 0.0) {
        // Retorna um objeto de erro
        return MakeErrorStatus(GridErr::DegenerateMesh, "Area=" + std::to_string(area));
    }
    return Status::OK();
}

// Exemplo C: Função que retorna Valor OU Erro (StatusOr)
StatusOr<double> calcularVolume(double raio) {
    if (raio < 0) {
        // Retorna erro (conversão implícita de Status para StatusOr)
        return MakeErrorStatus(GridErr::OptionsOutOfRange, "Raio negativo");
    }
    if (std::isnan(raio)) {
        return MakeErrorStatus(GridErr::NaNCoordinate);
    }
    
    // Retorna valor (double)
    return (4.0/3.0) * 3.14159 * std::pow(raio, 3);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "=== DEMONSTRAÇÃO COMPLETA: ERROR HANDLING ===\n\n";

    // Salva config original
    auto original_cfg = Config::get();

    // ------------------------------------------------------------------------
    // CENÁRIO 1: Tratamento Clássico (Exceptions & GridErrors)
    // ------------------------------------------------------------------------
    std::cout << "--- 1. Exceções com GridErrors ---\n";
    try {
        std::cout << "Tentando gerar malha invalida...\n";
        gerarMalha(-5); // Isso vai lançar
    } catch (const FVGException& e) {
        std::cout << "[CATCH] Exceção capturada!\n";
        std::cout << "  Msg: " << e.what() << "\n";
        
        // Verifica se é um erro de Grid
        if ((e.code() >> 16) == ErrorTraits<GridErr>::domain_id()) {
            std::cout << "  Dominio detectado: Grid\n";
        }
    }
    ErrorManager::flush(); // Limpa logs residuais

    // ------------------------------------------------------------------------
    // CENÁRIO 2: Padrão Funcional (Status / StatusOr)
    // ------------------------------------------------------------------------
    std::cout << "\n--- 2. Padrão Funcional (No Exceptions) ---\n";
    
    // 2.1 Testando Status simples
    Status st = validarGeometria(-10.0);
    if (!st.ok()) {
        std::cout << "[STATUS ERROR] Validação falhou de forma controlada.\n";
        std::cout << "  Erro: " << st.message() << "\n";
    } else {
        std::cout << "  Validação OK.\n";
    }

    // 2.2 Testando StatusOr<double>
    std::vector<double> raios = { 2.0, -1.0, 3.0 };
    std::cout << "\nCalculando volumes em lote:\n";
    
    for (double r : raios) {
        StatusOr<double> resultado = calcularVolume(r);
        
        if (resultado.ok()) {
            // Acesso seguro ao valor com .value()
            std::cout << "  Raio " << r << ": Vol = " << resultado.value() << "\n";
        } else {
            // Acesso ao erro com .status()
            std::cout << "  Raio " << r << ": FALHA -> " << resultado.status().message() << "\n";
        }
    }

    // ------------------------------------------------------------------------
    // CENÁRIO 3: Logger Customizado
    // ------------------------------------------------------------------------
    std::cout << "\n--- 3. Injeção de Logger Customizado ---\n";
    
    // Cria e configura o novo logger
    ErrorConfig custom_cfg = *original_cfg;
    custom_cfg.logger = std::make_shared<ConsoleLogger>();
    Config::set(custom_cfg);

    std::cout << "Logando erro via ConsoleLogger (deve aparecer com prefixo >>>):\n";
    
    // Usamos Policy::Status para não abortar o main com exceptions, apenas logar
    ErrorConfig no_throw_cfg = *Config::get();
    no_throw_cfg.policy = Policy::Status;
    Config::set(no_throw_cfg);

    FVG_ERROR(GridErr::ParallelBackendMissing);
    FVG_ERROR(FileErr::AccessDenied, {{"path", "/root/secret.txt"}});

    // ------------------------------------------------------------------------
    // CENÁRIO 4: i18n Dinâmico
    // ------------------------------------------------------------------------
    std::cout << "\n--- 4. Troca de Idioma em Runtime ---\n";
    ErrorConfig en_cfg = *Config::get();
    en_cfg.language = Language::EnUS;
    Config::set(en_cfg);

    FVG_ERROR(GridErr::ParallelBackendMissing); // Deve aparecer em Inglês

    // Restaura tudo
    Config::set(*original_cfg);
    std::cout << "\n=== FIM DA DEMONSTRAÇÃO ===\n";
    return 0;
}