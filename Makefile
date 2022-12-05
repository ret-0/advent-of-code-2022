CC = musl-gcc
CFLAGS = \
-pipe -std=c99 -D_POSIX_C_SOURCE=200112L \
-Wall -Wformat-security \
-static -Os -s -ffast-math -fdata-sections -ffunction-sections -flto \
-no-pie -fno-plt \
-Wl,--build-id=none -Wl,--gc-sections -Wl,-s \
-m64
LDFLAGS =

COMPILE = @clear ; $(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) && strip -s -R '.note.gnu.property' -R '.comment' $@
DEBUG   = @clear ; gcc -pipe -std=c99 -D_POSIX_C_SOURCE=200112L -Wall -Wformat-security -Og -g -fsanitize=undefined -fsanitize=address $@.c -o $@ $(LDFLAGS)

.PHONY: 01
01:
	$(COMPILE)

.PHONY: 02
02:
	$(COMPILE)

.PHONY: 03
03:
	$(COMPILE)

.PHONY: 04
04:
	$(COMPILE)

.PHONY: clean
clean:
	rm -f 01
	rm -f 02
	rm -f 03
	rm -f 04
