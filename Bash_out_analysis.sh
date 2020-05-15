cd /Users/harveybirch/GitHub/OCS_analysis
make clean
make
say "Starting analysis"


for f in /Users/harveybirch/GitHub/OCS_analysis/SURF_datastore/*root;
do
    fname=${f:54:51}
    echo "$fname"
    python2.7 runMe.py $fname
done

#mv Analysis_results/*PulseW_fit_parameters.txt Analysis_results/PulseWParam/
mv Analysis_results/*TrigW_fit_parameters.txt Analysis_results/TrigWParam/
mv Analysis_results/*TWxPW_fit_parameters.txt Analysis_results/TWxPWParam/
mv Analysis_results/*WidthsetTW_fit_parameters.txt Analysis_results/WidsetParam/
mv Analysis_results/*ph_fit_parameters.txt Analysis_results/NphParam/


for r in /Users/harveybirch/GitHub/OCS_analysis/Analysis_results/*Results.root;
do
    rname=${r:56:23}
    echo "$rname"
    python2.7 Plot.py Analysis_results/$rname
done

mv *.png /Users/harveybirch/GitHub/OCS_analysis/Analysis_results
mv *.root /Users/harveybirch/GitHub/OCS_analysis/Analysis_results
mv *.eps  /Users/harveybirch/GitHub/OCS_analysis/Analysis_results
echo "Analysis complete!"
say “Analysis complete!”
