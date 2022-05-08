TESTFILES="./src/testfiles/*"

for file in $TESTFILES
do
    echo "Testing $file"
    ./src/a.out $file
done

echo "Finish testing"