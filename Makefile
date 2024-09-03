cc = gcc

source_dir = src
include_dir = include
bin_dir = bin
lib_dir = libs

target = game.exe

source = $(shell find $(source_dir) -name "*.c")
object = $(patsubst $(source_dir)/%.c, $(bin_dir)/%.o, $(source))

flags = -I$(source_dir) -I$(include_dir) -Wall -Wextra -g
ld = -L$(lib_dir) -lglfw3 -lglad -lcglm -lgdi32
del = rm -rf
mkdir = mkdir -p
print = echo

file_nbr = $(words $(source))
file_count = 0

dep_files = $(patsubst $(source_dir)/%.c, $(bin_dir)/%.d, $(source))

all: $(target)

$(bin_dir):
	@$(mkdir) $@

$(target): $(object)
	@$(print) "[$(file_nbr)/$(file_nbr)] Linking $@"
	@$(cc) -g $^ -o $@ $(flags) $(ld)

$(bin_dir)/%.d: $(source_dir)/%.c | $(bin_dir)
	@$(mkdir) $(dir $@)
	@$(cc) $(flags) -MM -MT '$(bin_dir)/$*.o' $< > $@

-include $(dep_files)

$(bin_dir)/%.o: $(source_dir)/%.c $(bin_dir)/%.d | $(bin_dir)
	@$(mkdir) $(dir $@)
	@$(print) "[$(file_count)/$(file_nbr)] Compiling $<"
	@$(cc) -g -c $< -o $@ $(flags) $(ld)
	@$(eval file_count = $(shell expr $(file_count) + 1))

clean:
	@$(del) $(bin_dir)
	@$(print) "Removed all object files"
	@$(del) $(target)
	@$(print) "Removed $(target)"

re: clean all

run: all
	@./$(target)

.PHONY: all clean re run