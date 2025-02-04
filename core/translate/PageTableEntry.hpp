#pragma once

#include <stdint.h>
#include "include/AtlasTypes.hpp"
#include "core/translate/TranslateTypes.hpp"

#include "sparta/utils/LogUtils.hpp"

namespace atlas
{
    template <typename XLEN, MMUMode Mode> class PageTableEntry
    {
      public:
        PageTableEntry(XLEN pte) : pte_val_(pte) { decodePteFields_(); }

        XLEN getPte() const { return pte_val_; }

        XLEN getPpn() const { return ppn_; }

        XLEN getPpnField(const uint32_t idx) const { return ppn_fields_.at(idx); }

        // 2-bit Reserved for Supervisor, same for all modes
        uint32_t getRsw() const
        {
            return (pte_val_ & translate_types::Sv32::PteFields::rsw.bitmask)
                   >> translate_types::Sv32::PteFields::rsw.lsb;
        }

        // Dirty bit, same for all modes
        bool isDirty() const { return pte_val_ & translate_types::Sv32::PteFields::dirty.bitmask; }

        // Accessed bit, same for all modes
        bool isAccessed() const
        {
            return pte_val_ & translate_types::Sv32::PteFields::accessed.bitmask;
        }

        // Global bit, same for all modes
        bool isGlobal() const
        {
            return pte_val_ & translate_types::Sv32::PteFields::global.bitmask;
        }

        // User mode bit, same for all modes
        bool isUserMode() const
        {
            return pte_val_ & translate_types::Sv32::PteFields::user.bitmask;
        }

        // Execute bit, same for all modes
        bool canExecute() const
        {
            return pte_val_ & translate_types::Sv32::PteFields::execute.bitmask;
        }

        // Write bit, same for all modes
        bool canWrite() const { return pte_val_ & translate_types::Sv32::PteFields::write.bitmask; }

        // Read bit, same for all modes
        bool canRead() const { return pte_val_ & translate_types::Sv32::PteFields::read.bitmask; }

        // Valid bit, same fpr all modes
        bool isValid() const { return pte_val_ & translate_types::Sv32::PteFields::valid.bitmask; }

        bool isLeaf() const { return canRead() || canExecute(); }

      private:
        XLEN pte_val_;
        XLEN ppn_;
        std::vector<XLEN> ppn_fields_;

        void decodePteFields_()
        {
            if constexpr (Mode == MMUMode::SV32)
            {
                ppn_fields_.resize(translate_types::Sv32::num_ppn_fields);
                ppn_fields_[0] = (pte_val_ & translate_types::Sv32::PteFields::ppn0.bitmask)
                                 >> translate_types::Sv32::PteFields::ppn0.lsb;
                ppn_fields_[1] = (pte_val_ & translate_types::Sv32::PteFields::ppn1.bitmask)
                                 >> translate_types::Sv32::PteFields::ppn1.lsb;

                // Combine PPN1 and PPN0 to form the full PPN, 22 bits total
                ppn_ = (pte_val_
                        & (translate_types::Sv32::PteFields::ppn1.bitmask
                           | translate_types::Sv32::PteFields::ppn0.bitmask))
                       >> translate_types::Sv32::PteFields::ppn0.lsb;
            }
            else if constexpr (Mode == MMUMode::SV39)
            {
                ppn_fields_.resize(translate_types::Sv39::num_ppn_fields);
                ppn_fields_[0] = (pte_val_ & translate_types::Sv39::PteFields::ppn0.bitmask)
                                 >> translate_types::Sv39::PteFields::ppn0.lsb;
                ppn_fields_[1] = (pte_val_ & translate_types::Sv39::PteFields::ppn1.bitmask)
                                 >> translate_types::Sv39::PteFields::ppn1.lsb;
                ppn_fields_[2] = (pte_val_ & translate_types::Sv39::PteFields::ppn2.bitmask)
                                 >> translate_types::Sv39::PteFields::ppn2.lsb;

                // Combine PPN1 and PPN0 to form the full PPN, 22 bits total
                ppn_ = (pte_val_
                        & (translate_types::Sv39::PteFields::ppn2.bitmask
                           | translate_types::Sv39::PteFields::ppn1.bitmask
                           | translate_types::Sv39::PteFields::ppn0.bitmask))
                       >> translate_types::Sv39::PteFields::ppn0.lsb;
            }
            else
            {
                sparta_assert(false, "MMU mode is not currently supported!");
            }
        }
    };

    template <typename XLEN, MMUMode Mode>
    std::ostream & operator<<(std::ostream & os, const PageTableEntry<XLEN, Mode> & pte)
    {
        if constexpr (std::is_same_v<XLEN, RV64>)
        {
            os << HEX16(pte.getPte());
        }
        else
        {
            os << HEX8(pte.getPte());
        }
        return os;
    }
} // namespace atlas
