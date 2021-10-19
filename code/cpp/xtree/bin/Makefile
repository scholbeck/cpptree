bike:
	./tree --filename ../../../../data/seoulbike_8000.csv --coltypes nnnnnnnnn --target 2 --sep , --task regr --algorithm exhaustive --model linear --formula x6 --objective sse --minsize 50 --children 2 --maxdepth 10

credit:
	./tree --filename ../../../../data/credit.csv --coltypes cnccncccccccncccccccc --target 20 --sep , --task classif --algorithm exhaustive --objective gini --minsize 100 --children 2 --maxdepth 10

creditmult3:
	./tree --filename ../../../../data/credit.csv --coltypes cnccncccccccncccccccc --target 20 --sep , --task classif --algorithm random --objective gini --minsize 50 --children 3 --maxdepth 10

boston:
	./tree --filename ../../../../data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model mean --formula x1 --minsize 20 --children 2 --maxdepth 10

bostonlinear:
	./tree --filename ../../../../data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model linear --minsize 50 --children 2 --maxdepth 10

bostonmult3:
	./tree --filename ../../../../data/bh.data --target 13 --sep , --task regr --algorithm random --objective sse --model linear --formula x1 --minsize 50 --children 3 --maxdepth 10

bostonmult4:
	./tree --filename ../../../../data/bh.data --target 13 --sep , --algorithm random --objective sse --model linear --formula x1 --minsize 50 --children 4 --maxdepth 10

bostonmult5:
	./tree --filename ../../../../data/bh.data --target 13 --sep , --task regr --algorithm random --objective sse --model mean --minsize 100 --children 5 --maxdepth 10

iris:
	./tree --filename ../../../../data/iris.data --coltypes nnnnc --target 4 --sep , --task classif --algorithm exhaustive --objective gini --minsize 10 --children 2 --maxdepth 10

irismult:
	./tree --filename ../../../../data/iris.data --target 4 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3 --maxdepth 10

iris3way:
	./tree --filename ../../../../data/iris_10.data --target 4 --sep , --task classif --algorithm exhaustive --objective gini --model majorvote --minsize 1 --children 3

mushroom:
	./tree --filename ../../../../data/mushroom.data --target 22 --sep , --task classif --algorithm exhaustive --objective gini --minsize 100 --children 2

mushroommult3:
	./tree --filename ../../../../data/mushroom.data --target 22 --sep , --task classif --algorithm random --objective gini --minsize 1 --children 3

gt:
	./tree --filename ../../../../data/gt_data2.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 100 --maxdepth 10 --children 2

gtmult3:
	./tree --filename ../../../../data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 100 --maxdepth 10 --children 3

gtmult4:
	./tree --filename ../../../../data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 100 --maxdepth 10 --children 4

gtmult5:
	./tree --filename ../../../../data/gt_data2.csv --target 1 --sep , --task regr --algorithm random --objective sse --model mean --minsize 100 --maxdepth 10 --children 5

gt2:
	./tree --filename ../../../../data/gt_data1feature.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 1000 --children 2

gt3:
	./tree --filename ../../../../data/gt_2015.csv --target 1 --sep , --task regr --algorithm exhaustive --objective sse --model linear --formula x3 --minsize 100 --maxdepth 30 --children 2
