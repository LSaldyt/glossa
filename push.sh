# while true; do
#     read -p "Do you wish to install this program?" yn
#     case $yn in
#         [Yy]* ) make install; break;;
#         [Nn]* ) exit;;
#         * ) echo "Please answer yes or no.";;
#     esac
# done

echo Commiting repository and pushing..
git add *
read -p "Enter commit message:" commitmessage
git commit -m "$commitmessage"
git push origin master
echo Finished
