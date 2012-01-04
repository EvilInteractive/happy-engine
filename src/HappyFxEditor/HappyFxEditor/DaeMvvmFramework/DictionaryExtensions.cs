using System.Collections.Generic;

namespace DaeMvvmFramework
{
    public static class DictionaryExtensions
    {
        public static bool Find<TKey, TValue, TOutput>(this Dictionary<TKey, TValue> dict, TKey key, out TOutput output) 
            where TOutput : TValue
        {
            TValue value;
            bool hasValue = dict.TryGetValue(key, out value);
            output = hasValue ? (TOutput) value : default(TOutput);
            return hasValue;
        }

        public static TValue Find<TKey,TValue>(this Dictionary<TKey,TValue> dict, TKey key) where TValue:class
        {
            TValue value;
            dict.TryGetValue(key, out value);
            return value;
        }

        public static TValue Get<TKey, TValue>(this Dictionary<TKey, TValue> dict, TKey key, TValue defaultValue)
        {
            TValue value;
            if (!dict.TryGetValue(key, out value))
            {
                value = defaultValue;
            }
            return value;
        }
    }
}