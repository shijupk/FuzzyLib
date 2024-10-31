# FuzzyLib
FuzzyLib : Probabilistic Data structure and Algorithms
#1. Bloom Filter

A **Bloom filter** is a space-efficient probabilistic data structure that is used to test whether an element is a member of a set. It can produce **false positives** but **never false negatives**. This means that it might tell you an element is in the set even when it isn't, but if it says an element is not in the set, then the element is definitely not in the set.

## How a Bloom Filter Works

1. **Initialization**: The Bloom filter uses a bit array of length `m`, initialized to 0.
2. **Hash Functions**: It uses `k` independent hash functions. Each hash function maps an input to one of the `m` positions in the bit array uniformly.
3. **Adding Elements**:
   - To add an element, pass it through each of the `k` hash functions.
   - Each hash function outputs a position in the bit array, and each of these positions is set to 1.
4. **Checking Membership**:
   - To check if an element is in the set, pass it through each of the `k` hash functions.
   - If all of the positions in the bit array corresponding to the element's hashed values are set to 1, the Bloom filter reports the element might be in the set.
   - If any of these positions is 0, then the element is definitely not in the set.

## Properties of a Bloom Filter

- **False Positives**: A Bloom filter can produce false positives, where it incorrectly indicates that an element is in the set. The probability of a false positive depends on:
  - The size of the bit array (`m`)
  - The number of hash functions (`k`)
  - The number of inserted elements (`n`)

- **No False Negatives**: If a Bloom filter says an element is not in the set, it is guaranteed to be correct.

- **Space Efficiency**: Bloom filters are highly memory-efficient, making them ideal for applications where memory usage is a concern.

## Practical Applications of Bloom Filters

1. **Databases**: Quickly check if an element might be present in a large database to reduce the number of database lookups.
2. **Web Caching**: Bloom filters help in testing if an item might be in the cache, preventing unnecessary cache misses.
3. **Networking**: Bloom filters are used in distributed systems to efficiently manage membership of items across nodes.

## Example

Suppose you want to keep a blacklist of URLs for web security purposes. Instead of storing each URL, which could require significant memory, you can use a Bloom filter to determine if a URL is likely on the blacklist. This allows further checking only if the Bloom filter reports a positive result.

## Advantages and Disadvantages

### Advantages
- **Memory-efficient**: Uses less memory compared to storing the entire data set.
- **Fast**: Constant time complexity for insert and query operations.

### Disadvantages
- **False Positives**: May produce false positives.
- **Cannot Remove Elements**: Standard Bloom filters don’t allow removing elements, though variations like **Counting Bloom Filters** can.

## Calculating False Positive Probability

The probability of a false positive \( P \) after inserting `n` elements with `k` hash functions and a bit array size of `m` is approximately:

\[
P = \left(1 - e^{-kn/m}\right)^k
\]

This can be minimized by optimizing the values of `k` and `m` for a given number of elements `n`.

---

**Bloom filters** are valuable tools for optimizing memory usage in scenarios where approximate membership testing is sufficient. However, they should be used in cases where occasional false positives are acceptable.
