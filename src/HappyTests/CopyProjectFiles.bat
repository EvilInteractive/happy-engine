mkdir "build\%1"
copy "build\HappyTemplateProject\*" "build\%1\"

cd "build\%1"
ren "HappyTemplateProject.vcxproj" "%1.vcxproj.tmp"
ren "HappyTemplateProject.vcxproj.filters" "%1.vcxproj.filters.tmp"
ren "HappyTemplateProject.vcxproj.user" "%1.vcxproj.user"

cd ..\..
cd code

mkdir "%1\"
copy "HappyTemplateProject\*" "%1\"