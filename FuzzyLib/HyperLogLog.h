//#pragma once
//#include <vector>
//#include <cmath>
//#include <sstream>
//#include <stdexcept>
//#include <algorithm>
//
//#include "Common.h"
//
//namespace Fuzzy_lib
//{
//	namespace Hll
//	{
//        static constexpr double pow_2_32 = 4294967296.0; ///< 2^32
//        static constexpr double neg_pow_2_32 = -4294967296.0; ///< -(2^32)
//
//        template <typename T>
//        inline
//            void
//            write_be(char* p, T datum) noexcept {
//            std::copy_n(reinterpret_cast<const char*>(&datum), sizeof(T), p);
//        }
//
//        static inline size_t size_unsigned_var_int(unsigned int value) {
//            size_t size = 0;
//            while ((value & 0xFFFFFF80) != 0L) {
//                size++;
//                value >>= 7;
//            }
//            size++;
//            return size;
//        }
//
//        static inline size_t write_unsigned_var_int(unsigned int value, uint8_t* to) {
//            size_t size = 0;
//            while ((value & 0xFFFFFF80) != 0L) {
//                *to = (value & 0x7F) | 0x80;
//                value >>= 7;
//                to++;
//                size++;
//            }
//            *to = value & 0x7F;
//            size++;
//            return size;
//        }
//
//		class HyperLogLog
//		{
//        public:
//            /**
//    * Constructor
//    *
//    * @param[in] b bit width (register size will be 2 to the b power).
//    *            This value must be in the range[4,16].Default value is 4.
//    *
//    * @exception std::invalid_argument the argument is out of range.
//    */
//            HyperLogLog(uint8_t b = 4) :
//                b_(b), m_(1 << b), M_(m_, 0) {
//
//                if (b < 4 || 16 < b) {
//                    throw std::invalid_argument("bit width must be in the range [4,16]");
//                }
//
//                double alpha;
//                switch (m_) {
//                case 16:
//                    alpha = 0.673;
//                    break;
//                case 32:
//                    alpha = 0.697;
//                    break;
//                case 64:
//                    alpha = 0.709;
//                    break;
//                default:
//                    alpha = 0.7213 / (1.0 + 1.079 / m_);
//                    break;
//                }
//                alphaMM_ = alpha * m_ * m_;
//            }
//
//
//            void add_key(Data_types::bytes_view key) {
//                long hashed = Murmur_hash::(key, 0);
//                offer_hashed(hashed);
//            }
//
//        	void offer_hashed(uint64_t hash) {
//                uint32_t index = hash >> (64 - b_);
//                uint8_t rank = rho((hash << b_), 64 - b_);
//
//                if (rank > M_[index]) {
//                    M_[index] = rank;
//                }
//            }
//            /*
//     * Calculate the size of buffer returned by get_bytes().
//     */
//            size_t get_bytes_size() {
//                size_t size = 0;
//                size += sizeof(int); // version
//                size += size_unsigned_var_int(b_); // p; register width = b_.
//                size += size_unsigned_var_int(0); // sp; // sparse set = 0.
//                size += size_unsigned_var_int(0); // type;
//                size += size_unsigned_var_int(M_.size()); // register size;
//                size += M_.size();
//                return size;
//            }
//
//            temporary_buffer<uint8_t> get_bytes() {
//                // FIXME: add support to SPARSE format.
//                static constexpr int version = 2;
//
//                size_t s = get_bytes_size();
//                temporary_buffer<uint8_t> bytes(s);
//                size_t offset = 0;
//                // write version
//                write_be<int32_t>(reinterpret_cast<char*>(bytes.get_write() + offset), -version);
//                offset += sizeof(int);
//
//                // write register width
//                offset += write_unsigned_var_int(b_, bytes.get_write() + offset);
//                // NOTE: write precision value for sparse set (not supported).
//                offset += write_unsigned_var_int(0, bytes.get_write() + offset);
//                // write type (NORMAL always!)
//                offset += write_unsigned_var_int(0, bytes.get_write() + offset);
//                // write register size
//                offset += write_unsigned_var_int(M_.size(), bytes.get_write() + offset);
//                // write register
//                memcpy(bytes.get_write() + offset, M_.data(), M_.size());
//                offset += M_.size();
//
//                bytes.trim(offset);
//                if (s != offset) {
//                    throw std::runtime_error("possible overflow while generating cardinality metadata");
//                }
//                return bytes;
//            }
//
//            /**
//             * Estimates cardinality value.
//             *
//             * @return Estimated cardinality value.
//             */
//            double estimate() const {
//                double estimate;
//                double sum = 0.0;
//                for (uint32_t i = 0; i < m_; i++) {
//                    sum += 1.0 / pow(2.0, M_[i]);
//                }
//                estimate = alphaMM_ / sum; // E in the original paper
//                if (estimate <= 2.5 * m_) {
//                    uint32_t zeros = 0;
//                    for (uint32_t i = 0; i < m_; i++) {
//                        if (M_[i] == 0) {
//                            zeros++;
//                        }
//                    }
//                    if (zeros != 0) {
//                        estimate = m_ * log(static_cast<double>(m_) / zeros);
//                    }
//                }
//                else if (estimate > (1.0 / 30.0) * pow_2_32) {
//                    estimate = neg_pow_2_32 * log(1.0 - (estimate / pow_2_32));
//                }
//                return estimate;
//            }
//
//            /**
//             * Merges the estimate from 'other' into this object, returning the estimate of their union.
//             * The number of registers in each must be the same.
//             *
//             * @param[in] other HyperLogLog instance to be merged
//             *
//             * @exception std::invalid_argument number of registers doesn't match.
//             */
//            void merge(const HyperLogLog& other) {
//                if (m_ != other.m_) {
//                    std::stringstream ss;
//                    ss << "number of registers doesn't match: " << m_ << " != " << other.m_;
//                    throw std::invalid_argument(ss.str().c_str());
//                }
//                for (uint32_t r = 0; r < m_; ++r) {
//                    if (M_[r] < other.M_[r]) {
//                        M_[r] = other.M_[r];
//                    }
//                }
//            }
//
//            /**
//             * Clears all internal registers.
//             */
//            void clear() {
//                std::fill(M_.begin(), M_.end(), 0);
//            }
//
//            /**
//             * Returns size of register.
//             *
//             * @return Register size
//             */
//            uint32_t registerSize() const {
//                return m_;
//            }
//
//            /**
//             * Exchanges the content of the instance
//             *
//             * @param[in,out] rhs Another HyperLogLog instance
//             */
//            void swap(HyperLogLog& rhs) {
//                std::swap(b_, rhs.b_);
//                std::swap(m_, rhs.m_);
//                std::swap(alphaMM_, rhs.alphaMM_);
//                M_.swap(rhs.M_);
//            }
//
//            /**
//             * Dump the current status to a stream
//             *
//             * @param[out] os The output stream where the data is saved
//             *
//             * @exception std::runtime_error When failed to dump.
//             */
//            void dump(std::ostream& os) const {
//                os.write((char*)&b_, sizeof(b_));
//                os.write((char*)&M_[0], sizeof(M_[0]) * M_.size());
//                if (os.fail()) {
//                    throw std::runtime_error("Failed to dump");
//                }
//            }
//
//            /**
//             * Restore the status from a stream
//             *
//             * @param[in] is The input stream where the status is saved
//             *
//             * @exception std::runtime_error When failed to restore.
//             */
//            void restore(std::istream& is) {
//                uint8_t b = 0;
//                is.read((char*)&b, sizeof(b));
//                HyperLogLog tempHLL(b);
//                is.read((char*)&(tempHLL.M_[0]), sizeof(M_[0]) * tempHLL.m_);
//                if (is.fail()) {
//                    throw std::runtime_error("Failed to restore");
//                }
//                swap(tempHLL);
//            }
//
//        private:
//            uint8_t b_; ///< register bit width
//            uint32_t m_; ///< register size
//            double alphaMM_; ///< alpha * m^2
//            std::vector<uint8_t> M_; ///< registers
//
//            uint8_t rho(uint32_t x, uint8_t b) {
//                uint8_t v = 1;
//                while (v <= b && !(x & 0x80000000)) {
//                    v++;
//                    x <<= 1;
//                }
//                return v;
//            }
//
//
//		};
//	}
//}
//
//
