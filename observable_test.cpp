#include <iostream>
#include <string>

#include "observable.h"

namespace {
struct EventTag {};
struct DataTag {};
using EventObservable = lib::pattern::observable<EventTag, const std::string &>;
using DataObservable =
    lib::pattern::observable<DataTag, const void *, std::size_t>;

class MultiProducer : public EventObservable, public DataObservable {
  public:
    MultiProducer() = default;

    virtual ~MultiProducer() = default;
};

struct SingleTag {};
using SingleObservable =
    lib::pattern::observable<SingleTag, const std::string &>;

class SingleProducer : public SingleObservable {
  public:
    SingleProducer() = default;

    virtual ~SingleProducer() = default;
};
} // namespace

#include <gtest/gtest.h>
TEST(pattern, observer_multi) {
    MultiProducer producer;
    const std::string event = "12345";
    bool event_done[2] = {false};
    uint8_t data[10] = {0};
    bool data_done[2] = {false};

    producer.EventObservable::subscribe(
        [&event, &event_done](const std::string &a) {
            ASSERT_EQ(event, a);
            event_done[0] = true;
        });
    producer.EventObservable::subscribe(
        [&event, &event_done](const std::string &a) {
            ASSERT_EQ(event, a);
            event_done[1] = true;
        });
    producer.DataObservable::subscribe(
        [&data, &data_done](const void *data_pointer, std::size_t data_size) {
            ASSERT_EQ(reinterpret_cast<const void *>(data), data_pointer);
            ASSERT_EQ(sizeof data, data_size);
            data_done[0] = true;
        });
    producer.DataObservable::subscribe(
        [&data, &data_done](const void *data_pointer, std::size_t data_size) {
            ASSERT_EQ(reinterpret_cast<const void *>(data), data_pointer);
            ASSERT_EQ(sizeof data, data_size);
            data_done[1] = true;
        });

    producer.EventObservable::publish(event);
    producer.DataObservable::publish(data, sizeof data);

    for (const auto done : event_done) {
        ASSERT_TRUE(done);
    }
    for (const auto done : data_done) {
        ASSERT_TRUE(done);
    }
}
TEST(pattern, observer_single) {
    const std::string text = "123456789";
    bool done[2] = {false};
    // test for single observable object
    SingleProducer single_producer;
    single_producer.subscribe([&text, &done](const std::string &txt) {
        ASSERT_EQ(text, txt);
        done[0] = true;
    });
    single_producer.subscribe([&text, &done](const std::string &txt) {
        ASSERT_EQ(text, txt);
        done[1] = true;
    });
    single_producer.publish(text);
    for (const auto _done : done) {
        ASSERT_TRUE(_done);
    }
}