function push {
    echo Beginning commit
    git add *
    read -p "Enter commit message:" commitmessage
    git commit -m "$commitmessage"
    git push origin master
    echo Finished
}

echo Running Tests..
./run_tests.sh
result=$?
case $result in
    0 ) push; break;;
    * ) exit ;;
esac
