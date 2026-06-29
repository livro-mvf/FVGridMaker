
import importlib.util
import os
from datetime import datetime

project = "FVGridMaker"
author = "Joao Flavio Vieira de Vasconcellos"
copyright = f"{datetime.now().year}, {author}"
release = os.environ.get("FVG_VERSION", "1.0.0")
version = release.split("+", 1)[0]

root_doc = "index"
language = "pt_BR"
source_suffix = {
    ".rst": "restructuredtext",
    ".md": "markdown",
}

extensions = [
    "sphinx.ext.mathjax",
    "sphinx.ext.autosectionlabel",
]

for module in ("myst_parser", "sphinx_design"):
    if importlib.util.find_spec(module):
        extensions.append(module)

if importlib.util.find_spec("breathe"):
    extensions.append("breathe")

autosectionlabel_prefix_document = True

exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    "*.rst1",
    "pt/**",
    "tutorials/**",
]

templates_path = []
html_static_path = ["_static"]
html_css_files = ["custom.css"]

html_title = f"FVGridMaker {version}"
html_logo = "_static/fvgridmaker-logo.svg"
html_favicon = "_static/fvgridmaker-favicon.svg"

if importlib.util.find_spec("pydata_sphinx_theme"):
    html_theme = "pydata_sphinx_theme"
    html_theme_options = {
        "show_nav_level": 1,
        "navigation_depth": 3,
        "collapse_navigation": False,
        "show_toc_level": 2,
        "navbar_align": "left",
        "logo": {
            "text": "FVGridMaker",
        },
    }
else:
    html_theme = "alabaster"

doxygen_xml = os.environ.get("FVG_DOXYGEN_XML_DIR", "")

breathe_projects = {
    "FVGridMaker": doxygen_xml,
} if doxygen_xml else {}

breathe_default_project = "FVGridMaker"
breathe_default_members = (
    "members",
    "undoc-members",
)

# Breathe/Sphinx may emit duplicate C++ declarations for template deduction
# guides in the generated Doxygen index. Keep other warnings strict.
suppress_warnings = [
    "duplicate_declaration.cpp",
]

