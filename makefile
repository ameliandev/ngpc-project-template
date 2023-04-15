.SUFFIXES: .c .asm .rel .abs
NAME = main
OBJS = \
	main.rel \
	library.rel \
	core/rom/flash.rel \
	# core/class/global.rel \
	# core/class/pallete.rel \
	# core/utils/screen.rel \
	# core/rom/flash.rel \
	# core/fonts/v1.rel \
	# core/utils/print.rel \
	# controller/interface.rel \
	

$(NAME).ngc: makefile ngpc.lcf $(OBJS)
	wine $(THOME)/BIN/tulink.exe -la -o $(NAME).abs ngpc.lcf system.lib $(OBJS)
	wine $(THOME)/BIN/tuconv.exe -Fs24 $(NAME).abs
	wine $(THOME)/BIN/s242ngp.exe $(NAME).s24

.c.rel:
	wine $(THOME)/BIN/cc900.exe -c -O3 $< -o $@

clean:
	del *.rel
	del *.abs
	del *.map 
	del *.s24
