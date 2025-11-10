#!/bin/bash

case "$1" in
    "del")
        echo "Удаление папки build..."
        rm -rf build
        if [ $? -eq 0 ]; then
            echo "Папка build успешно удалена"
        else
            echo "Ошибка при удалении папки build"
        fi
        ;;
    "build")
        echo "Создание папки build..."
        mkdir -p build
        if [ $? -eq 0 ]; then
            echo "Папка build успешно создана"
        else
            echo "Ошибка при создании папки build"
        fi
        cd build
        cmake .. -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt"
        make -j4        
        cd ..
        ;;
    *)
        echo "Использование: $0 {del|build}"
        echo "  del   - удалить папку build"
        echo "  build - создать папку build"
        exit 1
        ;;
esac