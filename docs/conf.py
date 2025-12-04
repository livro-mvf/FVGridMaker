# docs/conf.py
# ---------------------------------------------------------------------------
# Configuração da documentação do FVGridMaker (Sphinx + Breathe + PyData)
# Layout inspirado na documentação do PETSc.
# Estrutura pensada para múltiplos idiomas:
#   - Conteúdo em português em: docs/pt/
#   - Futuro conteúdo em inglês em: docs/en/
# ---------------------------------------------------------------------------

import os
import sys
from datetime import datetime

# ---------------------------------------------------------------------------
# Caminho para o projeto (se precisar importar módulos Python no futuro)
# ---------------------------------------------------------------------------

# Raiz do repositório (ajuste se necessário)
sys.path.insert(0, os.path.abspath(".."))

# ---------------------------------------------------------------------------
# Informações básicas do projeto
# ---------------------------------------------------------------------------

project = "FVGridMaker"
author = "João Flávio Vieira de Vasconcellos"
current_year = datetime.now().year
copyright = (
    f"{current_year}, {author}. "
    "Distribuído sob a licença GNU GPL v3 ou posterior."
)

# Versão / release (pode ser sobrescrita pelo CMake via variável de ambiente)
release = os.environ.get("FVG_VERSION", "0.1.dev0")
version = release

# ---------------------------------------------------------------------------
# Documento raiz (importante para a estrutura pt/en)
# ---------------------------------------------------------------------------
# O Sphinx vai usar 'pt/index.rst' como ponto de entrada da documentação.
# Isso funciona com:
#   sphinx-build -b html . _build/pt
# ou com 'make html' gerado pelo quickstart.
#
# Em versões mais novas do Sphinx usa-se 'root_doc'; em versões antigas,
# 'master_doc'. Definimos os dois para compatibilidade.

root_doc = "pt/index"
master_doc = "pt/index"

# ---------------------------------------------------------------------------
# Extensões do Sphinx
# ---------------------------------------------------------------------------

extensions = [
    "breathe",                  # Integração Doxygen -> Sphinx
    "myst_parser",              # Suporte a Markdown (MyST)
    "sphinx_design",            # Grid cards, etc. (layout moderno)
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "sphinx.ext.mathjax",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosectionlabel",  # Para referências entre seções
]

# MyST: habilitar alguns recursos úteis
myst_enable_extensions = [
    "colon_fence",
    "deflist",
    "html_admonition",
    "html_image",
]

# Para referências entre seções
autosectionlabel_prefix_document = True

# ---------------------------------------------------------------------------
# Configurações gerais
# ---------------------------------------------------------------------------

templates_path = ["_templates"]

# Padrões a serem ignorados (relativos a docs/)
exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    # No futuro, se tiver en/ e você quiser ignorar algo específico, ajuste aqui.
]

# Idioma padrão desta build (documentação em português)
# Quando você for gerar uma versão em inglês em docs/en/,
# pode sobrescrever na linha de comando com: -D language=en
language = "pt_BR"

# ---------------------------------------------------------------------------
# Tema HTML (PyData, mas configurado para parecer com PETSc)
# ---------------------------------------------------------------------------

html_theme = "pydata_sphinx_theme"
html_static_path = ["_static"]

html_title = "FVGridMaker - Documentação"
html_logo = None  # Quando tiver um logo, troque aqui.

# Configurações do tema para se parecer mais com PETSc
html_theme_options = {
    # Configurações de navbar (mais limpa, como PETSc)
    "navbar_start": ["navbar-logo"],
    "navbar_center": ["navbar-nav"],
    "navbar_end": ["theme-switcher", "navbar-icon-links"],

    # Navegação lateral mais proeminente
    "show_nav_level": 2,
    "navigation_depth": 4,
    "collapse_navigation": False,
    "navigation_with_keys": True,

    # Header mais limpo
    "header_links_before_dropdown": 4,

    # Footer minimalista
    "footer_start": ["copyright"],
    "footer_center": ["sphinx-version"],
    "footer_end": [],

    # Layout mais compacto
    "show_toc_level": 2,
    "use_edit_page_button": False,

    # Ícones na barra superior
    "icon_links": [
        {
            "name": "GitHub",
            "url": "https://github.com/SEU-USUARIO/FVGridMaker",
            "icon": "fa-brands fa-github",
            "type": "fontawesome",
        },
    ],

    # Cores mais sóbrias (estilo PETSc) – chaves dependem da versão do tema
    "primary_color": "blue",
    "accent_color": "blue",

    # Anúncio (útil para indicar que a doc está em desenvolvimento)
    "announcement": f"FVGridMaker {release} - Documentação em desenvolvimento",
}

# Sidebars customizados
html_sidebars = {
    "**": ["sidebar-nav-bs", "search-field"],
}

# CSS customizado para se parecer mais com PETSc
html_css_files = [
    "custom.css",
]

# ---------------------------------------------------------------------------
# Breathe (integração Doxygen -> Sphinx)
# ---------------------------------------------------------------------------

doxygen_xml = os.environ.get("FVGRIDMAKER_DOXYGEN_XML", "")

breathe_projects = {}
if doxygen_xml:
    breathe_projects["FVGridMaker"] = doxygen_xml

breathe_default_project = "FVGridMaker"

# Configurações do Breathe para layout mais limpo
breathe_default_members = ("members", "private-members", "undoc-members")
breathe_show_define_initializer = True
breathe_show_enumvalue_initializer = True

# ---------------------------------------------------------------------------
# Configurações de autodoc / napoleon
# ---------------------------------------------------------------------------

napoleon_google_docstring = True
napoleon_numpy_docstring = True
napoleon_include_init_with_doc = True
napoleon_include_private_with_doc = True
napoleon_include_special_with_doc = True

# Ordenar membros por tipo, não alfabeticamente
autodoc_member_order = "groupwise"

# Incluir construtores na documentação
autoclass_content = "both"

# ---------------------------------------------------------------------------
# Intersphinx
# ---------------------------------------------------------------------------

intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
}

# ---------------------------------------------------------------------------
# Função de setup para configurações adicionais
# ---------------------------------------------------------------------------

def setup(app):
    # Adicionar CSS customizado
    app.add_css_file("custom.css")
