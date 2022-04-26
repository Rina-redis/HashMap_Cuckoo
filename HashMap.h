//
// Created by Никита on 25.04.2022.
//

#ifndef HASHMAPDOUBLEHASH_HASHMAP_H
#define HASHMAPDOUBLEHASH_HASHMAP_H

#include <iostream>
#include <cmath>
#include "DynamicArr.h"

class ArrayOfPrimeNum{
public:
    DynamicArr<int> PrimeArray;
    void set_newArr(int sizeforArray){
        int sizeOfArray = (sizeforArray) / 2 + 1;
        int *arr = new int[sizeOfArray];
        arr[0] = 2;
        for (int number = 3, j = 1; number < sizeforArray; number += 2, j++) {
            arr[j] = number;
        }
        for (int placeOfNum = 1, step = 3; placeOfNum < sqrt(sizeOfArray); placeOfNum++, step += 2) {
            if (arr[placeOfNum] != 0) {
                for (int i = placeOfNum + step; i < sizeOfArray; i += step) {
                    arr[i] = 0;
                }
            }
        }
        for (int i = 0;i < sizeOfArray; i++){
            if (arr[i] != 0) {
                PrimeArray.push_back(arr[i]);
            }
        }
        delete [] arr;
        return;
    }
    int FindNearest(int searchElement){
        int leftBord=0;
        int rightBord=PrimeArray.size()-1;
        while (rightBord > leftBord){
            int mid=(leftBord+rightBord)/2;
            if(PrimeArray.Array[mid]==searchElement){
                return PrimeArray.Array[mid];
            }
            if(PrimeArray.Array[mid]>searchElement){
                rightBord=mid-1;
            }
            else{
                leftBord=mid+1;
            }
        }
        if(rightBord != PrimeArray.size()-1)
            return PrimeArray.Array[rightBord+1];
        else
            return PrimeArray.Array[rightBord];
    }
    int get_last(){
        return PrimeArray.Array[(PrimeArray.size())-1];
    }
};
template<typename T>
class HashMap{
private:
    struct HashSigment{
        T data;
        bool Empty = 1;
        long long key;
    };
    void swap(HashSigment* a,HashSigment* b){
        HashSigment temp = *a;
        *a = *b;
        *b = temp;
    }
    ArrayOfPrimeNum PrimeArr;

    short a = 2 , b;
    int size=0;
    int arrlen=11;


    float coefload=(float)size/(float)arrlen;

    HashSigment *array = new HashSigment[arrlen];

    void resize_arr(){
        if(PrimeArr.get_last() < arrlen*2.3){
            PrimeArr.set_newArr(arrlen*4);
        }
        int newArrLen = PrimeArr.FindNearest(arrlen*2.3);
        HashSigment *new_arr = new HashSigment[newArrLen];

        int oldarrlen = arrlen;
        arrlen = newArrLen;

        for(int i = 0;i < oldarrlen;i++){
            if(array[i].Empty == 0){
                insertforNewArr(array[i].key,array[i].data,new_arr);
            }
        }
        delete[] array;
        array = new_arr;
    }

    long long hash(long long key,int i)
    {
        const float fi = (1 + sqrt(5))/2;
        switch(i){
            case 0:{
                return ((a * key + b) % 9149658775000477) % arrlen;
            }
            case 1:{
                return (long long)(fmod(0.6*(float)key,1.0) * arrlen);
            }
            case 2:{
                return key % arrlen;
            }
            case 3:{
                return (long long)(fi*key) % arrlen;
            }
        }
    }
    void insertforNewArr(long long key,const T& value,HashSigment* arr){
        HashSigment A{value,0,key};

        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(arr[index].Empty == 1){
                arr[index]=A;
                return;
            }
        }
        long long index = hash(key,rand()%4);
        swap(&arr[index],&A);
        insertforNewArr(A.key, A.data,arr);
    }
    void insertCycle(long long key, const T& value,int i){
        if(i == 20){
            rehash();
            insert(key,value);
            return;
        }
        i++;
        HashSigment A{value,0,key};

        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(array[index].Empty == 1){
                size++;
                array[index]=A;
                return;
            }
        }
        long long index = hash(key,rand()%4);
        swap(&array[index],&A);
        insertCycle(A.key, A.data,i);
    }
    void rehash(){
        static bool flag = 1;
        if(flag == 0){
            for(int i = arrlen-1; i>=0; i--){
                if(array[i].Empty == 0){
                    size--;
                    array[i].Empty = 1;
                    insert(array[i].key,array[i].data);
                }
            }
            flag = 1;
            return;
        }
        for(int i = 0; i < arrlen; i++){
            if(array[i].Empty == 0){
                size--;
                array[i].Empty = 1;
                insert(array[i].key,array[i].data);
            }
        }
        flag = 0;
        return;
    }
public:
    HashMap():b(rand()%19 + 1){
        PrimeArr.set_newArr(arrlen*4);
        a = PrimeArr.FindNearest(rand()%12);
    }

    ~HashMap(){
        delete [] array;
    }

    int sizeH(){
        return size;
    }

    void insert(long long key,const T& value){
        coefload = (float)size/(float)arrlen;
        if(coefload>0.5){
            resize_arr();
        }
        HashSigment A{value,0,key};

        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(array[index].key == key and array[index].Empty == 0){
                array[index]=A;
                return;
            }
        }
        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(array[index].Empty == 1){
                size++;
                array[index]=A;
                return;
            }
        }
        long long index = hash(key,rand()%3);
        swap(&array[index],&A);
        insertCycle(A.key, A.data,0);
    }

    void erase(long long key){
        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(array[index].key == key and array[index].Empty == 0){
                size = size-1;
                array[index].Empty = 1;
                return;
            }
        }
    }

    T* find(long long key){
        for(int i = 0; i < 4; i++){
            long long index = hash(key,i);
            if(array[index].key == key and array[index].Empty == 0){
                return &array[index].data;
            }
        }
        return nullptr;
    }
};
#endif //HASHMAPDOUBLEHASH_HASHMAP_H
