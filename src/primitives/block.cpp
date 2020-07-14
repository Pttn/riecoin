// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2013-2020 The Riecoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>

uint256 CBlockHeader::GetHash() const
{
    // The hash is done after swapping nTime and nBits
    uint8_t blockData[112];
    memcpy(&blockData[0], &nVersion, 4);
    memcpy(&blockData[4], hashPrevBlock.begin(), 32);
    memcpy(&blockData[36], hashMerkleRoot.begin(), 32);
    memcpy(&blockData[68], &nBits, 4);
    memcpy(&blockData[72], &nTime, 8);
    memcpy(&blockData[80], ArithToUint256(nOffset).begin(), 32);
    // Hash the whole Block Header
    return Hash(blockData, &blockData[112]);
}

uint256 CBlockHeader::GetHashForPoW() const
{
    // The hash is done after swapping nTime and nBits
    uint8_t blockData[80];
    memcpy(&blockData[0], &nVersion, 4);
    memcpy(&blockData[4], hashPrevBlock.begin(), 32);
    memcpy(&blockData[36], hashMerkleRoot.begin(), 32);
    memcpy(&blockData[68], &nBits, 4);
    memcpy(&blockData[72], &nTime, 8);
    // Hash the Block Header without nOffset
    return Hash(blockData, &blockData[80]);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%lu, nBits=0x%08x, nOffset=%s, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nOffset.ToString().c_str(),
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
