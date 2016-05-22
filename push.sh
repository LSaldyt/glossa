echo Beginning commit
git add *
read -p "Enter commit message:" commitmessage
git commit -m "$commitmessage"
git push origin master
echo Finished
