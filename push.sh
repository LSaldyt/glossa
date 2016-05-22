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
read -p "Tests Completed. Are you satisfied with the results?" answer
    case $answer in
        [Yy]* ) push; break;;
        [Nn]* ) echo Exiting..; break;;
        * ) echo "Please answer yes or no.";;
    esac
