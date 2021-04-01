//------------------------------------------------------------------------------
//! Specifies the syntax of the supported database as well as includes struct
//! for a Dictionary entry.
//! 
//! @file   language.h
//! @author tralf-strues
//! @date   2021-04-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#pragma once

#include <stdio.h>

const char* POS_SPECIFIERS[PARTS_OF_SPEECH_TOTAL - 1] = { 
            "a.",    /* adjective          */
            "adv.",  /* adverb             */
            "pl.",   /* noun plural        */
            "n.",    /* noun               */
            "p. p."  /* past participle    */
            "imp."   /* past simple        */
            "p. pr." /* present participle */
            "prep.", /* preposition        */
            "pron.", /* pronoun            */
            "v. t."  /* verb transitive    */ 
            "v. i."  /* verb intransitive  */ 
        };  

enum PartOfSpeech
{
    UNSPECIFIED,

    ADJECTIVE,
    ADVERB,
    NOUN_PLURAL,
    NOUN,
    PAST_PARTICIPLE,
    PAST_SIMPLE,
    PRESENT_PARTICIPLE,
    PREPOSITION,
    PRONOUN,
    VERB_INTRANSITIVE,
    VERB_TRANSITIVE,

    PARTS_OF_SPEECH_TOTAL
};

struct Definition
{
    PartOfSpeech partOfSpeech;
    char*        text;
};

struct DictEntry
{
    const char* word;
    size_t      definitionsCount;
    Definition* definitions; 
};