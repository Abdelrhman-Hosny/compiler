TESTFILES="./src/testfiles/*"

echo "Running tests..."
echo "Make sure you are in the project root directory"

for file in $TESTFILES
do
    echo "Testing $file"
    ./src/a.exe $file
done

echo "Finish testing"