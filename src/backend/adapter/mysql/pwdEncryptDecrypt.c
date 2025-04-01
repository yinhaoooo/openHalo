/*-------------------------------------------------------------------------
 *
 * pwdEncryptDecrypt.c
 *	  MySQL adapter pwdEncryptDecrypt routines
 *
 * 
 * 版权所有 (c) 2019-2025, 易景科技保留所有权利。
 * Copyright (c) 2019-2025, Halo Tech Co.,Ltd. All rights reserved.
 * 
 * 易景科技是Halo Database、Halo Database Management System、羲和数据
 * 库、羲和数据库管理系统（后面简称 Halo ）、openHalo软件的发明人同时也为
 * 知识产权权利人。Halo 软件的知识产权，以及与本软件相关的所有信息内容（包括
 * 但不限于文字、图片、音频、视频、图表、界面设计、版面框架、有关数据或电子文
 * 档等）均受中华人民共和国法律法规和相应的国际条约保护，易景科技享有上述知识
 * 产权，但相关权利人依照法律规定应享有的权利除外。未免疑义，本条所指的“知识
 * 产权”是指任何及所有基于 Halo 软件产生的：（a）版权、商标、商号、域名、与
 * 商标和商号相关的商誉、设计和专利；与创新、技术诀窍、商业秘密、保密技术、非
 * 技术信息相关的权利；（b）人身权、掩模作品权、署名权和发表权；以及（c）在
 * 本协议生效之前已存在或此后出现在世界任何地方的其他工业产权、专有权、与“知
 * 识产权”相关的权利，以及上述权利的所有续期和延长，无论此类权利是否已在相
 * 关法域内的相关机构注册。
 *
 * This software and related documentation are provided under a license
 * agreement containing restrictions on use and disclosure and are 
 * protected by intellectual property laws. Except as expressly permitted
 * in your license agreement or allowed by law, you may not use, copy, 
 * reproduce, translate, broadcast, modify, license, transmit, distribute,
 * exhibit, perform, publish, or display any part, in any form, or by any
 * means. Reverse engineering, disassembly, or decompilation of this 
 * software, unless required by law for interoperability, is prohibited.
 * 
 * This software is developed for general use in a variety of
 * information management applications. It is not developed or intended
 * for use in any inherently dangerous applications, including applications
 * that may create a risk of personal injury. If you use this software or
 * in dangerous applications, then you shall be responsible to take all
 * appropriate fail-safe, backup, redundancy, and other measures to ensure
 * its safe use. Halo Corporation and its affiliates disclaim any 
 * liability for any damages caused by use of this software in dangerous
 * applications.
 * 
 *
 * IDENTIFICATION
 *	  src/backend/adapter/mysql/pwdEncryptDecrypt.c
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"
#include "common/cryptohash.h"
#include "libpq/crypt.h"

#include "adapter/mysql/pwdEncryptDecrypt.h"

const char *mysql_native_password_prefix = "mysql_native_password:";

bool
mysNativePwdEncrypt(const char *pwd, char *buf)
{
    uint8 sum[SHA1_DIGEST_LENGTH];
    pg_cryptohash_ctx *ctx;
	int	q; 
    int w;
    size_t pwdLen;
    static char *hex;

    pwdLen = strlen(pwd);
    strcpy(buf, "mysql_native_password:");

	ctx = pg_cryptohash_create(PG_SHA1);
	if (ctx == NULL)
		return false;
    if (pg_cryptohash_init(ctx) < 0 ||
		pg_cryptohash_update(ctx, (uint8*)pwd, pwdLen) < 0 ||
		pg_cryptohash_final(ctx, sum, sizeof(sum)) < 0)
	{
		pg_cryptohash_free(ctx);
		return false;
	}
    if (pg_cryptohash_init(ctx) < 0 ||
		pg_cryptohash_update(ctx, sum, SHA1_DIGEST_LENGTH) < 0 ||
		pg_cryptohash_final(ctx, sum, sizeof(sum)) < 0)
	{
		pg_cryptohash_free(ctx);
		return false;
	}

    hex = "0123456789abcdef";
	for (q = 0, w = MYS_NATIVE_PASSWORD_PREFIX_LEN; q < SHA1_DIGEST_LENGTH; q++)
	{
		buf[w++] = hex[(sum[q] >> 4) & 0x0F];
		buf[w++] = hex[sum[q] & 0x0F];
	}
	buf[w] = '\0';

    pg_cryptohash_free(ctx);
    
    return true;
}

bool
checkMysNativeAuth(const char *clientPwd, const char *shadowPassTemp)
{
    int i;
    int temp;
    char shadow_pass[SHA1_DIGEST_LENGTH];
    char *salt = "v\021\001\037wg\021\002z\020X}VNSCgc|\021";
    uint8 sum[SHA1_DIGEST_LENGTH];
    pg_cryptohash_ctx *ctx;

    for(i = MYS_NATIVE_PASSWORD_PREFIX_LEN; i < MYS_NATIVE_PASSWORD_LEN; i += 2)
    {
        int index = (i - 22) / 2;
        char cha_high4 = shadowPassTemp[i];
        char cha_low4 = shadowPassTemp[i+1];
        if (cha_high4 >= '0' && cha_high4 <= '9')
        {
            shadow_pass[index] = ((cha_high4 - '0') << 4) & 0xF0;
        }
        else if (cha_high4 >= 'a' && cha_high4 <= 'f')
        {
            shadow_pass[index] = (((cha_high4 - 'a' + 10) << 4) & 0xF0);
        }

        // low 4 bit
        if (cha_low4 >= '0' && cha_low4 <= '9')
        {
            shadow_pass[index] += ((cha_low4 - '0') & 0x0F);
        }
        else if (cha_low4 >= 'a' && cha_low4 <= 'f')
        {
            shadow_pass[index] += ((cha_low4 - 'a' + 10) & 0x0F);
        }
    }

	ctx = pg_cryptohash_create(PG_SHA1);
	if (ctx == NULL)
		return false;
    
    if (pg_cryptohash_init(ctx) < 0 ||
		pg_cryptohash_update(ctx, (uint8*)salt, strlen(salt)) < 0 ||
        pg_cryptohash_update(ctx, (uint8*)shadow_pass, SHA1_DIGEST_LENGTH) < 0 ||
		pg_cryptohash_final(ctx, sum, sizeof(sum)) < 0)
	{
		pg_cryptohash_free(ctx);
		return false;
	}

    temp = 0;
    while(temp < SHA1_DIGEST_LENGTH)
    {
        sum[temp] = sum[temp] ^ clientPwd[temp];
        temp++;
    }

    if (pg_cryptohash_init(ctx) < 0 ||
		pg_cryptohash_update(ctx, sum, SHA1_DIGEST_LENGTH) < 0 ||
		pg_cryptohash_final(ctx, sum, SHA1_DIGEST_LENGTH) < 0)
	{
		pg_cryptohash_free(ctx);
		return false;
	}

    pg_cryptohash_free(ctx);

    return (memcmp(shadow_pass, sum, SHA1_DIGEST_LENGTH) == 0);
}

