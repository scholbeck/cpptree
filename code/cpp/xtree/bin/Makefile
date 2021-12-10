bench:
	./cpptree --filename ../../../../data/10000obs.csv --coltypes nnnnnn --target 1 --sep , --task regr --algorithm exhaustive --model mean --objective sse --minsize 30 --children 2 --maxdepth 30

bike:
	./cpptree --filename ../../../../data/seoulbike_8000.csv --coltypes nnnnnnnnn --target 2 --sep , --task regr --algorithm exhaustive --model linear --formula x6 --objective sse --minsize 2 --children 2 --maxdepth 30

boston:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 30 --children 2 --maxdepth 5

bostonlinear:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm exhaustive --objective sse --model linear --formula x6 --minsize 30 --children 2 --maxdepth 5

bostonmult3:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --task regr --algorithm random --objective sse --model linear --formula x6 --minsize 30 --children 3 --maxdepth 5

bostonmult4:
	./cpptree --filename ../../../../data/bh.data --target 14 --sep , --algorithm random --objective sse --model linear --formula x6 --minsize 30 --children 4 --maxdepth 5

iris:
	./cpptree --filename ../../../../data/iris.data --coltypes nnnnc --target 5 --sep , --task classif --algorithm exhaustive --objective gini --minsize 10 --children 2 --maxdepth 10 | tee iris.txt

irismult:
	./cpptree --filename ../../../../data/iris.data --target 4 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3 --maxdepth 10

iris3way:
	./cpptree --filename ../../../../data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./cpptree --filename ../../../../data/mushroom.data --target 1 --sep , --task classif --algorithm exhaustive --objective gini --minsize 30 --children 2

