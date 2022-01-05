#ifndef _TKET_TESTS_TokenSwapping_TestUtils_FullTsaTesting_H_
#define _TKET_TESTS_TokenSwapping_TestUtils_FullTsaTesting_H_

#include "Architecture/Architecture.hpp"
#include "TokenSwapping/PartialTsaInterface.hpp"
#include "TokenSwapping/RNG.hpp"
#include "TokenSwapping/SwapListOptimiser.hpp"

namespace tket {
namespace tsa_internal {
namespace tests {

// Only for testing FULL TSAs, which guarantee to find a solution.
class FullTsaTesting {
 public:
  FullTsaTesting();

  /// Will use the RiverFlowPathFinder
  /// (which needs an RNG).
  void add_problems(
      const Architecture& arch, const std::vector<VertexMapping>& problems,
      const std::string& name, RNG& rng, PartialTsaInterface& full_tsa);

  /// A summary of the statistics.
  std::string str() const;

 private:
  // For various optimisation passes, we check how well they did,
  // and we record when a particular one beats
  struct Counts {
    size_t total_swaps = 0;
    size_t problems_where_this_was_the_joint_winner = 0;
    size_t problems_where_this_was_the_clear_winner = 0;

    // Reset this with each new calculated solution; this checks whether
    // newly calculated solutions really are just a permutation of an existing
    // solution.
    std::vector<Swap> sorted_swaps;
  };

  size_t m_total_lower_bounds = 0;
  size_t m_number_of_problems = 0;
  size_t m_number_of_tokens = 0;
  SwapList m_swap_list;
  SwapListOptimiser m_optimiser;
  std::vector<Counts> m_counts_list;
  std::string m_name;
  std::string m_prev_tsa_name;

  enum class AllowEmptySwaps { YES, NO };

  // Check that the swaps currently stored in m_swap_list are correct,
  // and store the data in m_counts_list (if the index is not too big).
  void check_solution(
      size_t counts_list_index, VertexMapping vertex_mapping,
      size_t lower_bound, AllowEmptySwaps allow_empty_swaps);

  // Check that the swaps currently stored in m_swap_list are correct.
  // Check also that they are a reordering of those
  // already calculated and stored in m_counts_list, at the given index.
  void check_equivalent_good_solution(
      size_t existing_index, VertexMapping vertex_mapping,
      AllowEmptySwaps allow_empty_swaps);

  // In m_counts_list, the swaps for i1 should be <= the swaps for i2.
  void test_order(size_t index1, size_t index2) const;

  void complete_counts_list_for_single_problem();
};

}  // namespace tests
}  // namespace tsa_internal
}  // namespace tket
#endif