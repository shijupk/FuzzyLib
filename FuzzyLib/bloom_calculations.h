#pragma once
#include <format>
#include <vector>
#include "Common.h"
namespace Fuzzy_lib
{
	/**
 * The following calculations are taken from:
 * http://www.cs.wisc.edu/~cao/papers/summary-cache/node8.html
 * "Bloom Filters - the math"
 *
 * This class's static methods are meant to facilitate the use of the Bloom
 * Filter class by helping to choose correct values of 'bits per element' and
 * 'number of hash functions, k'.
 */
	namespace Filter
	{
        /**
     * A wrapper class that holds two key parameters for a Bloom Filter: the
     * number of hash functions used, and the number of buckets per element used.
     */
        struct bloom_specification final {
            int K; // number of hash functions.
            int buckets_per_element;

            bloom_specification(int k, int buckets_per_element) : K(k), buckets_per_element(buckets_per_element) { }

            operator std::string() {
                return std::format("bloom_specification(K={:d}, buckets_per_element={:d})", K, buckets_per_element);
            }
        };

        int constexpr min_buckets = 2;
        int constexpr min_k = 1;
        int constexpr EXCESS = 20;

        extern const std::vector<std::vector<double>> probs;
        extern const std::vector<int> opt_k_per_buckets;

        /**
     * Given the number of buckets that can be used per element, return a
     * specification that minimizes the false positive rate.
     *
     * @param buckets_per_element The number of buckets per element for the filter.
     * @return A spec that minimizes the false positive rate.
     */
        inline bloom_specification compute_bloom_spec(int buckets_per_element) {
            FUZZY_ASSERT(buckets_per_element >= 1);
            FUZZY_ASSERT(buckets_per_element <= static_cast<int>(probs.size()) - 1);
            return bloom_specification(opt_k_per_buckets[buckets_per_element], buckets_per_element);
        }

        /**
     * Given a maximum tolerable false positive probability, compute a Bloom
     * specification which will give less than the specified false positive rate,
     * but minimize the number of buckets per element and the number of hash
     * functions used.  Because bandwidth (and therefore total bitvector size)
     * is considered more expensive than computing power, preference is given
     * to minimizing buckets per element rather than number of hash functions.
     *
     * @param max_buckets_per_element The maximum number of buckets available for the filter.
     * @param max_false_pos_prob The maximum tolerable false positive rate.
     * @return A Bloom Specification which would result in a false positive rate
     * less than specified by the function call
     * @throws unsupported_operation_exception if a filter satisfying the parameters cannot be met
     */
        inline bloom_specification compute_bloom_spec(int max_buckets_per_element, double max_false_pos_prob) {
            FUZZY_ASSERT(max_buckets_per_element >= 1);
            FUZZY_ASSERT(max_buckets_per_element <= static_cast<int>(probs.size()) - 1);

            auto max_k = static_cast<int>(probs[max_buckets_per_element].size()) - 1;

            // Handle the trivial cases
            if (max_false_pos_prob >= probs[min_buckets][min_k]) {
                return bloom_specification(2, opt_k_per_buckets[2]);
            }

            if (max_false_pos_prob < probs[max_buckets_per_element][max_k]) {
                throw Exceptions::unsupported_operation_exception(std::format("Unable to satisfy {:f} with {:d} buckets per element", max_false_pos_prob, max_buckets_per_element));
            }

            // First find the minimal required number of buckets:
            int buckets_per_element = 2;
            int K = opt_k_per_buckets[2];

            while (probs[buckets_per_element][K] > max_false_pos_prob) {
                buckets_per_element++;
                K = opt_k_per_buckets[buckets_per_element];
            }
            // Now that the number of buckets is sufficient, see if we can relax K
            // without losing too much precision.
            while (probs[buckets_per_element][K - 1] <= max_false_pos_prob) {
                K--;
            }

            return bloom_specification(K, buckets_per_element);
        }

        /**
     * Calculates the maximum number of buckets per element that this implementation
     * can support.  Crucially, it will lower the bucket count if necessary to meet
     * BitSet's size restrictions.
     */
        inline int max_buckets_per_element(long num_elements) {
            num_elements = std::max(1l, num_elements);

            auto v = std::numeric_limits<long>::max() - EXCESS;
            v = v / num_elements;

            if (v < 1) {
                throw Exceptions::unsupported_operation_exception(std::format("Cannot compute probabilities for {:d} elements.", num_elements));
            }
            return std::min(probs.size() - 1, static_cast<size_t>(v));
        }

        /**
         * Retrieves the minimum supported bloom_filter_fp_chance value
         * if compute_bloom_spec() above is attempted with bloom_filter_fp_chance
         * lower than this, it will throw an unsupported_operation_exception.
         */
        inline double min_supported_bloom_filter_fp_chance() {
            return probs.back().back();
        }


	}
}


