gcc main.c help.c utility.c modification_commands.c reading_analytics_commands.c -o mygit.exe

:: ==========================================
:: ПРИМЕР ИСПОЛЬЗОВАНИЯ ВСЕХ КОМАНД 
:: ==========================================
::
:: 1. Инициализация
:: .\mygit init
::
:: 2. Работа с файлами (создайте test1.txt и test2.txt)
:: .\mygit add test1.
:: .\mygit add test2.txt
:: .\mygit status
:: .\mygit remove test2.txt
:: .\mygit status
::
:: 3. Сохранение (Коммит)
:: .\mygit commit "first_commit"
::
:: 4. Аналитика
:: .\mygit log (выводит цепочку всех коммитов)
:: .\mygit log --n 1
:: (измените test1.txt руками)
:: .\mygit diff (должны вывестись отлия от прошлого файла построчно)
::
:: 5. Изменение существующего
:: (скопируйте хэш коммита из log)
:: .\mygit checkout ВАШ_ХЭШ
::
:: 6. Справки
:: .\mygit help
:: .\mygit help_prog_version commit
:: ==========================================