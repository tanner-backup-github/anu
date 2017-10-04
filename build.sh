clear
BASE=~/Desktop/anu/

cd "$BASE"

for i in *.*; do
    [ -f "$i" ] || break
    without_extension="${i%%.*}"
    if [[ "$i" == *.s ]]; then
	nasm -felf32 "$i" -o "$BASE/obuild/$without_extension.o"
    elif [[ "$i" == *.c ]]; then 
        i686-elf-gcc -D"$1" -g -c "$i" -o "$BASE/obuild/"$without_extension".o" -I"$BASE/include" -std=gnu99 -ffreestanding -O0 -Wall -Wextra -Werror
    fi
done

i686-elf-gcc -T "$BASE/link.ld" -g -o "$BASE/obuild/os.bin" -ffreestanding -O0 -nostdlib "$BASE/obuild/"*.o -lgcc
qemu-system-i386 -serial stdio -kernel "$BASE/obuild/os.bin" -display "$2" -m "$3" # dumb gtk output (sdl doesn't work with qemu monitor)
