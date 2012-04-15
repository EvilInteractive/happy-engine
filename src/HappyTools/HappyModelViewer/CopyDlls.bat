cd "%1"
cd "..\..\..\3thParty\dll\%2"
echo ---Copying Dll's
copy "*.dll" "%1"
copy "*.exe" "%1"
copy "*.pdb" "%1"
echo ------Done