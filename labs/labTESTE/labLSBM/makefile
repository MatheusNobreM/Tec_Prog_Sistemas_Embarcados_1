# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------
CHAIN   := arm-none-eabi

# Flags de compilação
CFLAGS  := -std=c99 -Wall -ffreestanding -nostdlib
IPATH   := -Iinc/

# ---------------------------------------------------------------------------
# Diretórios
# ---------------------------------------------------------------------------
SRC     := src/
OBJDIR  := obj/
BINDIR  := bin/

# ---------------------------------------------------------------------------
# Fontes detectados automaticamente
# (qualquer *.c ou *.s dentro de src/)
# ---------------------------------------------------------------------------
SRCS_C := $(wildcard $(SRC)*.c)
SRCS_S := $(wildcard $(SRC)*.s)

# Converte src/foo.c|s -> obj/foo.o
OBJS := $(patsubst $(SRC)%.c,$(OBJDIR)%.o,$(SRCS_C)) \
        $(patsubst $(SRC)%.s,$(OBJDIR)%.o,$(SRCS_S))

# ---------------------------------------------------------------------------
# Alvo padrão
# ---------------------------------------------------------------------------
all: app

# ---------------------------------------------------------------------------
# Link final -> gera main.elf / spl.boot
# ---------------------------------------------------------------------------
app: $(OBJDIR)main.elf
	@echo "=> Gerando SPL binário…"
	@mkdir -p $(BINDIR)
	$(CHAIN)-objcopy $< $(BINDIR)spl.boot -O binary
#	# comente a linha abaixo se não quiser copiar via sudo
#	sudo cp $(BINDIR)spl.boot /srv/tftp/Interrupt.bin

# ---------------------------------------------------------------------------
# Link intermediário
# ---------------------------------------------------------------------------
$(OBJDIR)main.elf: $(OBJS) | $(OBJDIR)
	@echo "=> Linkando:"
	$(CHAIN)-ld $^ -T $(SRC)memmap.ld -o $@

# ---------------------------------------------------------------------------
# Regras genéricas
# ---------------------------------------------------------------------------
# .c -> .o
$(OBJDIR)%.o: $(SRC)%.c | $(OBJDIR)
	@echo "CC  $< -> $@"
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $< -o $@

# .s -> .o
$(OBJDIR)%.o: $(SRC)%.s | $(OBJDIR)
	@echo "AS  $< -> $@"
	$(CHAIN)-as $(IPATH) $< -o $@

# ---------------------------------------------------------------------------
# Diretórios automáticos
# ---------------------------------------------------------------------------
$(OBJDIR):
	mkdir -p $@

# ---------------------------------------------------------------------------
# Utilidades
# ---------------------------------------------------------------------------
.PHONY: clean dump

clean:
	rm -rf $(OBJDIR) $(BINDIR)

dump: $(OBJDIR)main.elf
	$(CHAIN)-objdump -D $<
