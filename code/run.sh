if [ "$1" == "1" ]; then
  ./abc -C "bugfind testcases/golden/c17_G.blif testcases/faulty/c17_F.blif"
elif [ "$1" == "2" ]; then
  ./abc -C "bugfind testcases/golden/c432_G.blif testcases/faulty/c432_F.blif"
elif [ "$1" == "3" ]; then
  ./abc -C "bugfind testcases/golden/c499_G.blif testcases/faulty/c499_F.blif"
elif [ "$1" == "4" ]; then
  ./abc -C "bugfind testcases/golden/c880_G.blif testcases/faulty/c880_F.blif"
elif [ "$1" == "5" ]; then
  ./abc -C "bugfind testcases/golden/c1355_G.blif testcases/faulty/c1355_F.blif"
fi

echo "I don't know what your choice is . " && exit 0
