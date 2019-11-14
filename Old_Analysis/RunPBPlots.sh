for f in /user/hbirch/LZ_experiment/Firmware_testing/FST_SEP/Data/FPGA*;
  do 
     [ -d $f ] && cd "$f" && echo Entering into $f
     ls | wc -l 
     for rootfile in *.root
     do
	 echo "$rootfile"
	 echo "$f/$rootfile"
	 foldername=${PWD##*/}
	 echo "$foldername"
	 FPGANum=${foldername:4:1}
	 ChNum=${foldername:7:1}
	 echo "$FPGANum"
	 echo "$ChNum"
	 root -q /user/hbirch/LZ_experiment/Firmware_testing/FST_SEP/FST_Analysis_V2.C+(\"$f/$rootfile\",$FPGANum,$ChNum)
     done
     #     pdfnup NphVStrwidth$foldername.pdf PulseWidthVSnph$foldername.pdf --nup 1x2 --no-landscape -o joint.pdf
     #     pdfnup PulseWidthVSnph$foldername.pdf twVSlnph_with_residuals_$foldername.pdf --nup 1x2 --no-landscape -o joint.pdf
     #     pdfnup nph$foldername.pdf joint.pdf -o PulserBoard$PBNum.pdf
     #     cp PulserBoard$PBNum.pdf /user/hbirch/Documents/LZ_experiment/FST_Old_Firmware_plots
     #     rm joint.pdf
     cd /user/hbirch/LZ_experiment/Firmware_testing/FST_SEP/Data/
done;

