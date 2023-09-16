#include<stdio.h>

double getPrice() {
    return basePrice() * discountFactor();
}

double discountFactor() {
    if (basePrice() > 1000) discountFactor() = 0.95;
    else discountFactor() = 0.98;
    return;
}

int basePrice() {
    return _quantity * _itemPrice;
}

