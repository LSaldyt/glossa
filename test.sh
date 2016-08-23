function runtests {
    cd tests/build
    ./suite
    result=$?
    echo Finished
    exit $result
}

echo Running application ..
make all
make test_suite
makesuccess=$?

case $makesuccess in
    0 ) runtests ;;
    * ) exit $makesuccess;;
esac
