/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "simplep2p.h"

static bool enet_ok = false;

void register_simplep2p_types() {

	if (enet_initialize() != 0) {
		ERR_PRINT("ENet initialization failure");
	} else {
		enet_ok = true;
	}
        ObjectTypeDB::register_type<SimpleP2P>();
}

void unregister_simplep2p_types() {
   if (enet_ok)
	enet_deinitialize();
}