#pragma once
#include <vector>
#include <algorithm>

template<typename TObject, typename TCreateReq>
class QueuedManager {
protected:
    std::vector<TObject*> objects;
    std::vector<TCreateReq> createQueue;
    std::vector<TObject*> deleteQueue;

    virtual ~QueuedManager() = default;

    virtual TObject* createFromRequest(const TCreateReq& req) = 0;
    virtual void destroyObject(TObject* obj) = 0;

public:
    void applyQueues() {
        for (TObject* obj : deleteQueue) {
            auto it = std::find(objects.begin(), objects.end(), obj);
            if (it != objects.end()) {
                destroyObject(obj);
                objects.erase(it);
            }
        }
        deleteQueue.clear();

        for (const auto& req : createQueue) {
            TObject* obj = createFromRequest(req);
            if (obj)
                objects.push_back(obj);
        }
        createQueue.clear();
    }
};