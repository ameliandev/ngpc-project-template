.SUFFIXES: .c .asm .rel .abs
NAME = main
OBJS = \
	main.rel \
	library.rel \
	core/rom/flash.rel \

CURRENT_DIR := $(abspath .)
OUTPUT_DIR = bin

$(NAME).ngp: makefile ngpc.lcf $(OBJS)
	tulink -la -o $(NAME).abs ngpc.lcf system.lib $(OBJS)
	tuconv -Fs24 $(NAME).abs
	s242ngp $(NAME).s24

.c.rel:
	cc900 -c -O3 $< -o $@

winclean:
	
	@rm -f $(OUTPUT_DIR)/*.abs 
	@rm -f $(OUTPUT_DIR)/*.map
	@rm -f $(OUTPUT_DIR)/*.s24
	@rm -f $(OUTPUT_DIR)/*.ngc
	@rm -f $(OUTPUT_DIR)/*.ngp
	@rm -rf *.rel

winmove_files:
	@if [ ! -d "$(OUTPUT_DIR)" ]; then mkdir $(OUTPUT_DIR); fi
	@if [ -f "$(NAME).ngp" ]; then mv $(NAME).ngp $(OUTPUT_DIR)/$(NAME).ngc; fi
	@if [ -f "$(NAME).abs" ]; then mv $(NAME).abs $(OUTPUT_DIR)/$(NAME).abs; fi
	@if [ -f "$(NAME).rel" ]; then mv $(NAME).rel $(OUTPUT_DIR)/$(NAME).rel; fi
	@if [ -f "$(NAME).s24" ]; then mv $(NAME).s24 $(OUTPUT_DIR)/$(NAME).s24; fi