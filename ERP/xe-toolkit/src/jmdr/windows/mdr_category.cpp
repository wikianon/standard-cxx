#include <time.h>
#include "mdr_category.h"

namespace mdr {

mdr_host_instance_map_t mdr_category_t::cats_per_host;
CRITICAL_SECTION        mdr_category_t::cats_mux;

mdr_cat_refresh_times_t mdr_category_t::cat_times_map;
CRITICAL_SECTION        mdr_category_t::cat_times_mux;

static bool initialization_needed = true;

void mdr_category_init(void)
{
  if (initialization_needed) {
    InitializeCriticalSection(&mdr_category_t::cats_mux);
    InitializeCriticalSection(&mdr_category_t::cat_times_mux);
    initialization_needed = false;
  }
}

mdr_category_t::mdr_category_t(const char *name, const char *hostname)
{
  remote_host = hostname;

  EnterCriticalSection(&cats_mux);
    const char *const localname = mdr_t::mdr_local_host.c_str();
    bool localhost = (remote_host == 0);
    mdr_host_instance_node_t hnode =
      cats_per_host.find(localhost ? localname : remote_host);

    // assure that there is a host node for this request
    if (hnode == cats_per_host.end()) {
      cats_per_host[localhost ? localname : remote_host].clear();
      hnode = cats_per_host.find(localhost ? localname : remote_host);
    }
  LeaveCriticalSection(&cats_mux);

  cat_inst_map = &hnode->second;

  mdr_cat_instance_node_t inode = cat_inst_map->find(name);

  if (inode == cat_inst_map->end()) {

    // allocate mdr_t object here
    mdr_obj = mdr_t::mdr_get_mdr_object(name, remote_host);

    if (mdr_obj == 0)
      throw mdr_exception_t(
        "mdr_category_t::mdr_category_t: invalid name: %s", name);

    (*cat_inst_map)[name] = mdr_obj;
  } else {
    mdr_obj = inode->second;
    mdr_refresh();
  }

  mdr_category_name = name;
  mdr_is_singleton = mdr_obj->mdr_is_singleton;
  mdr_field_count = mdr_obj->mdr_field_count;

  EnterCriticalSection(&cat_times_mux);
    // track refresh times on each category and govern to >= 1/sec
    mdr_cat_refresh_node_t rn = cat_times_map.find(mdr_obj);
    if (rn == cat_times_map.end())
      cat_times_map[mdr_obj] = 0;
  LeaveCriticalSection(&cat_times_mux);
}

const std::string &mdr_category_t::mdr_field_name(int field_number)
{
  return mdr_obj->mdr_field_name(field_number);
}

const std::string &mdr_category_t::mdr_field_explanation(int field_number, std::string &explanation)
{
  return mdr_obj->mdr_field_explanation(field_number, explanation);
}

int mdr_category_t::mdr_field_name_to_number(const char *name)
{
  return mdr_obj->mdr_field_name_to_number(name);
}

mdr_keyset_t &mdr_category_t::mdr_all_field_names(mdr_keyset_t &name_set)
{
  return mdr_obj->mdr_all_field_names(name_set);
}

mdr_keyset_t &mdr_category_t::mdr_supported_field_names(mdr_keyset_t &name_set)
{
  return mdr_obj->mdr_supported_field_names(name_set);
}

const mdr_value_t &mdr_category_t::mdr_field_value(int field_number)
{
  return mdr_obj->mdr_field_value(field_number);
}

const mdr_value_t &
  mdr_category_t::mdr_field_value(const char *key, int field_number)
{
  return mdr_obj->mdr_field_value(key, field_number);
}

void mdr_category_t::mdr_refresh(void)
{
  mdr_cat_refresh_node_t rn;

  EnterCriticalSection(&cat_times_mux);
    rn = cat_times_map.find(mdr_obj);
    // # of milliseconds since boot
    DWORD now = GetTickCount();
    char *thresh_env = 0;
    DWORD threshold = CLOCKS_PER_SEC;
    size_t tsz = sizeof(thresh_env);

    _dupenv_s(&thresh_env, &tsz, "MDR_INTERVAL_THRESHOLD");

    // allow setting of threshold through environment
    if (thresh_env != 0) {
      threshold = (DWORD) atol(thresh_env);
      free(thresh_env);
    }

    // do not update category more than once per threshold
    if ((now - rn->second) >= threshold) {
      mdr_obj->mdr_refresh();
      cat_times_map[mdr_obj] = now;
    }
  LeaveCriticalSection(&cat_times_mux);
}

const mdr_keyset_t &mdr_category_t::mdr_get_instance_names(mdr_keyset_t &name_set)
{
  if (mdr_obj->mdr_is_singleton)
    throw mdr_exception_t(
      "mdr_category_t::mdr_get_instance_names: %s is not an n-tuple",
        mdr_obj->mdr_category_name.c_str());

  return mdr_obj->mdr_keyset(name_set);
}

// release resources associated with this host
void mdr_category_t::mdr_release_host(const char *hostname)
{
  EnterCriticalSection(&cats_mux);
    // find the host
    mdr_host_instance_node_t hnode =
      cats_per_host.find(hostname ? hostname : mdr_t::mdr_local_host.c_str());
    if (hnode != cats_per_host.end()) {
      // release all of the categories for this host
      mdr_cat_instance_map_t &cmap = hnode->second;
      mdr_cat_instance_node_t cnode;

      for(cnode=cmap.begin(); cnode != cmap.end(); cnode++) {
        mdr_t *mdrp = cnode->second;

        if (mdrp)
          delete mdrp;
      }

      // leave the host node pointing to an empty map
      cmap.clear();
    }
  LeaveCriticalSection(&cats_mux);
}

}
