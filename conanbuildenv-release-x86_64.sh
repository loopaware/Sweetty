script_folder="/home/fredrick/vscodium-workspace/sweetty-gl"
echo "echo Restoring environment" > "$script_folder/deactivate_conanbuildenv-release-x86_64.sh"
for v in PATH LD_LIBRARY_PATH
do
   is_defined="true"
   value=$(printenv $v) || is_defined="" || true
   if [ -n "$value" ] || [ -n "$is_defined" ]
   then
       echo export "$v='$value'" >> "$script_folder/deactivate_conanbuildenv-release-x86_64.sh"
   else
       echo unset $v >> "$script_folder/deactivate_conanbuildenv-release-x86_64.sh"
   fi
done

export PATH="$PATH:/home/fredrick/.conan2/p/b/elfutac9195ff82751/p/bin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/fredrick/.conan2/p/b/elfutac9195ff82751/p/lib"