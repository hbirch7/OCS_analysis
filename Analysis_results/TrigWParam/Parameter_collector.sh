for file in *TrigW_fit_parameters.txt;do
    COUNTER=0 
    while [  $COUNTER -lt 24 ]; do
	cat $file
	/n
	let COUNTER=COUNTER+1 
    done
done < $file;


#cd Analysis_results;
#echo "Searching for TrigW_parameter text files...";
#lr *TrigW_fit_parameters.txt;
#echo "Found these files."
#for f in *;
#do echo "File: ${f}";
#echo "reading..."
#exec<${f}
#value=0
#while read line
#do
#   echo ${line};
#done



