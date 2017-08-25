void Access (bool group_access, bool Pet_Access, String chat, String id) {
  Aut = 0;
  if (Pet_Access) {
    if (id == Pet)Aut = 1;
  }
  if (id == Dakota)Aut = 1;
  if (id == Owner_1)Aut = 1;
  if (group_access){
    if (chat == Gruppe_id) {
      if (id != Pet)Aut = 1;
    }  
  }
}
