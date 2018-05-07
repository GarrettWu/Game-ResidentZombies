#pragma once

class CoinChangeListener {
public:
    //result: 0:success; 1:failed
    virtual void onCoinChanged() {
        
    }
};

class PurchaseEndListener {
public:
    //result: 0:success; 1:failed
    virtual void onPurchaseEnded(int itemId) {
        
    }
};