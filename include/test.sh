CFILE=".*\.[c,h].*"
for file in `ls $1`
do
	if [[ ${file} =~ ${CFILE} ]]; then
		echo "======="${file}
	else
		echo ${file}
	fi
done
