mapfile -t CPP_FILES < <(find . -type f -name "*.[ch]pp")
printf "%s\n" "${CPP_FILES[@]}"
for file in "${CPP_FILES[@]}"
do
   clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -i $file
done