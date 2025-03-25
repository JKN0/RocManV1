sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 main.c
sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 matrix.c
sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 game.c
sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 sound.c
sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 ir_rec.c
sdcc -c --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 eeprom.c
sdcc --model-small --no-xinit-opt --iram-size 256 --xram-size 8192 -o RocMan.ihx main.rel matrix.rel game.rel sound.rel ir_rec.rel eeprom.rel
packihx RocMan.ihx > RocMan.hex
