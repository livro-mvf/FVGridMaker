#!/usr/bin/env python3
"""
Script para verificar a configuração do target FVGridMaker
"""

import json
import subprocess
import sys
from pathlib import Path

def check_target_info(build_dir: Path):
    """Verifica informações do target CMake"""
    cmake_info = {}
    
    # Verifica se o diretório de build existe
    if not build_dir.exists():
        print(f"❌ Diretório de build não encontrado: {build_dir}")
        return False
    
    # Executa cmake --build --target help para ver targets disponíveis
    try:
        result = subprocess.run(
            ["cmake", "--build", str(build_dir), "--target", "help"],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.returncode == 0:
            print("✅ Targets disponíveis:")
            for line in result.stdout.split('\n'):
                if line.strip() and '...' in line:
                    target = line.split('...')[0].strip()
                    print(f"   - {target}")
    
    except subprocess.TimeoutExpired:
        print("⚠️  Timeout ao verificar targets")
    
    return True

def main():
    project_root = Path(__file__).parent.parent
    build_dir = project_root / "build"
    
    print("🔍 Verificando configuração do target FVGridMaker...")
    print(f"   Diretório do projeto: {project_root}")
    print(f"   Diretório de build: {build_dir}")
    print()
    
    if not check_target_info(build_dir):
        sys.exit(1)
    
    print("\n✅ Verificação concluída com sucesso!")

if __name__ == "__main__":
    main()