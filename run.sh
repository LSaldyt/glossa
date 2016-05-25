echo Running application ..
make all
makeresult=$?

function run {
    cd build
    ./Compiler
    echo Finished
}

case $makeresult in
    0 ) run;;
    * ) exit $makeresult;;
esac
