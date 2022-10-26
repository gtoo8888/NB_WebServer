#!/bin/bash

ROOT_DIR=$PWD
OUT_DIR=/bin/test_presure_result

RES_TEMP_FILE=../1.txt
RES_FILE_NEMA=webbench_test_result.txt

res_file=${ROOT_DIR}${OUT_DIR}/${RES_FILE_NEMA}
rm -r ${res_file}
echo $ROOT_DIR
mkdir -p ${OUT_DIR}


test_presure_fun(){
    test_client=$1
    test_time=$2

    ./webbench -c ${test_client} -t ${test_time}  http://127.0.0.1:8888/ > ${RES_TEMP_FILE}
    echo "./webbench -c ${test_client} -t  ${test_time}  http://127.0.0.1:8888/" >> ${res_file}
    cat ${RES_TEMP_FILE} | grep "Speed" >> ${res_file}
    cat ${RES_TEMP_FILE} | grep "Requests" >> ${res_file}
    echo " " >> ${res_file}
    rm -f ${RES_TEMP_FILE}
}

cd test_presure/webbench-1.5

test_presure_fun 500 1
test_presure_fun 5000 1
test_presure_fun 10000 1
# test_presure_fun 20000 1

test_presure_fun 500 5
test_presure_fun 5000 5
test_presure_fun 10000 5
# test_presure_fun 20000 5

test_presure_fun 500 10
test_presure_fun 5000 10
test_presure_fun 10000 10
# test_presure_fun 20000 10

cd -



