// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/PathQueue.h"

void PathQueue::Enqueue(int32 Data) {
	Path.Add(Data);
}

int32 PathQueue::Dequeue() {
	if (Path.IsEmpty()) {
		return -1;
	}
	int32 tmp = Path[0];
	Path.RemoveAt(0);
	return tmp;
}

int32 PathQueue::Peek() {
	if (Path.IsEmpty()) {
		return -1;
	}
	return Path[0];
}

int32 PathQueue::Num() const {
	return Path.Num();
}

bool PathQueue::IsEmpty() const {
	return Path.IsEmpty();
}

TArray<int32> PathQueue::TestGetPath() {
	return Path;
}
