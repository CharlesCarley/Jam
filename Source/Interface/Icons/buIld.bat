@echo off
python gen_md.py > Icons.md
python gen_rc.py > Icons.qrc
python gen_log.py
