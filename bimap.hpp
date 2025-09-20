#include <QDebug>
#include <QHash>

template <typename KeyT, typename ValueT>
class BiMap {
 public:
  int count() const { return keyToValue_.count(); }

  void insert(const KeyT &key, const ValueT &value) {
    if (keyToValue_.contains(key) || valueToKey_.contains(value)) {
      remove_by_key(key);
      remove_by_value(value);
    }
    keyToValue_.insert(key, value);
    valueToKey_.insert(value, key);
  }

  void remove_by_key(const KeyT &key) {
    if (!keyToValue_.contains(key)) {
      return;
    }
    ValueT value = keyToValue_.value(key);

    keyToValue_.remove(key);
    valueToKey_.remove(value);
  }

  void remove_by_value(const ValueT &value) {
    if (!valueToKey_.contains(value)) {
      return;
    }
    KeyT key = valueToKey_.value(value);

    valueToKey_.remove(value);
    keyToValue_.remove(key);
  }
  void clear() {
    keyToValue_.clear();
    valueToKey_.clear();
  }
  const KeyT key(const ValueT &value) const {
    return valueToKey_.value(value);
  }
  const ValueT value(const KeyT &key) const {
    return keyToValue_.value(key);
  }
  bool is_contains_key(const KeyT &key) const {
    return keyToValue_.contains(key);
  }
  bool is_contains_value(const ValueT &value) const {
    return valueToKey_.contains(value);
  }
  void print() const {
    qDebug() << "key->value:" << keyToValue_;
    qDebug() << "value->key:" << valueToKey_;
  }

 private:
  QHash<KeyT, ValueT> keyToValue_;
  QHash<ValueT, KeyT> valueToKey_;
};
