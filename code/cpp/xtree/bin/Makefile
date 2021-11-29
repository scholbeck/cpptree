bike:
	./cpptree --filename ../../../../data/seoulbike_8000.csv --coltypes nnnnnnnnn --target 2 --sep , --task regr --algorithm exhaustive --model linear --formula x6 --objective sse --minsize 50 --children 2 --maxdepth 10

boston:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 30 --children 2 --maxdepth 4 | tee bostonmean.txt

bostonlinear:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm exhaustive --objective sse --model linear --formula x6 --minsize 30 --children 2 --maxdepth 2 | tee bostonlinear.txt

bostonmult3:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm random --objective sse --model linear --formula x6 --minsize 30 --children 3 --maxdepth 4 | tee bostonmult3.txt

bostonmult4:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --algorithm random --objective sse --model linear --formula x6 --minsize 30 --children 4 --maxdepth 4 | tee bostonmult4.txt

iris:
	./cpptree --filename ../../../../data/iris.data --coltypes nnnnc --target 5 --sep , --task classif --algorithm exhaustive --objective gini --minsize 10 --children 2 --maxdepth 10 | tee iris.txt

irismult:
	./cpptree --filename ../../../../data/iris.data --target 4 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3 --maxdepth 10

iris3way:
	./cpptree --filename ../../../../data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./cpptree --filename ../../../../data/mushroom.data --target 22 --sep , --task classif --algorithm exhaustive --objective gini --minsize 30 --children 2 | tee musthroom.txt

