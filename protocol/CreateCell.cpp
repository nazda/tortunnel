/*-
 * Copyright (c) 2009, Moxie Marlinspike
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of this program nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "CreateCell.h"
#include "HybridEncryption.h"

#include <iostream>
#include "../util/Util.h"

CreateCell::CreateCell(uint16_t circuitId, DH *dh, RSA *onionKey) :
  Cell(circuitId, (unsigned char)0x01)
{
  const BIGNUM *pub_key;
  DH_get0_key(dh, &pub_key, NULL);
  int            plaintextPayloadLength = BN_num_bytes(pub_key);
  unsigned char* plaintextPayload       = (unsigned char*)malloc(plaintextPayloadLength);

  int            encryptedPayloadLength;
  unsigned char* encryptedPayload;

  BN_bn2bin(pub_key, plaintextPayload);

  HybridEncryption::encrypt(plaintextPayload, plaintextPayloadLength,
			    &encryptedPayload, &encryptedPayloadLength,
			    onionKey);

//   std::cerr << "Create Cell Encrypted Payload Length: " << encryptedPayloadLength << std::endl;

  append(encryptedPayload, encryptedPayloadLength);
  
//   std::cerr << "Sending Create Cell: " << std::endl;
//   Util::hexDump(getPayload(), getPayloadSize());

  free(plaintextPayload);
  free(encryptedPayload);
}
