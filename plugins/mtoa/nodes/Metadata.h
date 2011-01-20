#ifndef ARNOLD_RENDER_OPTIONS_H
#define ARNOLD_RENDER_OPTIONS_H

#include <ai_metadata.h>

AtBoolean  MAiMetaDataGetBool (const AtNodeEntry *entry, const char *param, const char *name, AtBoolean *value);
AtBoolean  MAiMetaDataGetInt (const AtNodeEntry *entry, const char *param, const char *name, AtInt *value);
AtBoolean  MAiMetaDataGetFlt (const AtNodeEntry *entry, const char *param, const char *name, AtFloat *value);
//AtBoolean  MAiMetaDataGetPnt (const AtNodeEntry *entry, const char *param, const char *name, AtPoint *value);
//AtBoolean  MAiMetaDataGetVec (const AtNodeEntry *entry, const char *param, const char *name, AtVector *value);
//AtBoolean  MAiMetaDataGetPnt2 (const AtNodeEntry *entry, const char *param, const char *name, AtPoint2 *value);
//AtBoolean  MAiMetaDataGetRGB (const AtNodeEntry *entry, const char *param, const char *name, AtColor *value);
AtBoolean  MAiMetaDataGetStr (const AtNodeEntry *entry, const char *param, const char *name, char *value);

AtParamValue MAiParamGetDefault(const AtNodeEntry *entry, const AtParamEntry* paramEntry);

#endif
