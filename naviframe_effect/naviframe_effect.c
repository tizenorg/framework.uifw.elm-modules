/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Flora License, Version 1.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include <elementary_config.h>
#endif

#include <Elementary.h>
#include <elm_widget_naviframe.h>

struct custom_effect
{
   Evas_Object *obj;
   Evas_Coord   w, h;
   Evas_Map    *map;

   struct _effect
     {
        double zoom;
        int    alpha;
     }from, to;
};

//Tizen Only
#ifdef ELM_FEATURE_WEARABLE_RECTANGLE
static void
_tizen_transit_del_cb(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Naviframe_Item *it = data;

   it->tizen_transit = NULL;
}

static void
_tizen_transit_del_cb2(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Evas_Coord x, y, w, h;
   Elm_Naviframe_Item *it = data;

   evas_object_geometry_get(WIDGET(it), &x, &y, &w, &h);
   evas_object_move(VIEW(it), x, y);
   evas_object_resize(VIEW(it), w, h);
   evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
   it->tizen_transit2 = NULL;
}

EAPI void
tizen_pop_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit, *transit2;

   if (!it->tizen_zoom) return;

   if (it->tizen_transit)
     {
        elm_transit_del(it->tizen_transit);
        it->tizen_transit = NULL;
     }

   if (it->tizen_transit2)
     {
        elm_transit_del(it->tizen_transit2);
        it->tizen_transit2 = NULL;
     }

   transit = elm_transit_add();
   if (!transit) return;

   transit2 = elm_transit_add();
   if (!transit2)
     {
        elm_transit_del(transit);
        return;
     }

   it->tizen_transit = transit;
   it->tizen_transit2 = transit2;

   //Transition for zoom 0.92 times
   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_zoom_add(transit, 0.92, 0.92);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_SINUSOIDAL);
   elm_transit_tween_mode_factor_set(transit, 1.7, 0.0);
   elm_transit_duration_set(transit, 0.05);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_del_cb, it);

   //Transition for zoom from 0.92 to 1.0 times
   elm_transit_object_add(transit2, VIEW(it));
   elm_transit_effect_zoom_add(transit2, 0.92, 1.0);
   elm_transit_tween_mode_set(transit2, ELM_TRANSIT_TWEEN_MODE_SINUSOIDAL);
   elm_transit_tween_mode_factor_set(transit2, 1.7, 0.0);
   elm_transit_duration_set(transit2, 0.2);
   elm_transit_smooth_set(transit2, EINA_FALSE);
   elm_transit_del_cb_set(transit2, _tizen_transit_del_cb2, it);

   elm_transit_chain_transit_add(transit, transit2);
   elm_transit_go(transit);
}

EAPI void
tizen_push_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit, *transit2;

   if (!it->tizen_zoom) return;

   if (it->tizen_transit)
     {
        elm_transit_del(it->tizen_transit);
        it->tizen_transit = NULL;
     }

   if (it->tizen_transit2)
     {
        elm_transit_del(it->tizen_transit2);
        it->tizen_transit2 = NULL;
     }

   transit = elm_transit_add();
   if (!transit) return;

   transit2 = elm_transit_add();
   if (!transit2)
     {
        elm_transit_del(transit);
        return;
     }

   it->tizen_transit = transit;
   it->tizen_transit2 = transit2;

   //Transition for zoom from 1.0 to 0.92 times
   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_zoom_add(transit, 1.0, 0.92);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_SINUSOIDAL);
   elm_transit_tween_mode_factor_set(transit, 1.7, 0.0);
   elm_transit_duration_set(transit, 0.2);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_del_cb, it);

   //Transition for zoom 0.92 times
   elm_transit_object_add(transit2, VIEW(it));
   elm_transit_effect_zoom_add(transit2, 0.92, 0.92);
   elm_transit_tween_mode_set(transit2, ELM_TRANSIT_TWEEN_MODE_SINUSOIDAL);
   elm_transit_tween_mode_factor_set(transit2, 1.7, 0.0);
   elm_transit_duration_set(transit2, 0.05);
   elm_transit_smooth_set(transit2, EINA_FALSE);
   elm_transit_del_cb_set(transit2, _tizen_transit_del_cb2, it);

   elm_transit_chain_transit_add(transit, transit2);
   elm_transit_go(transit);
}
#elif ELM_FEATURE_WEARABLE_CIRCLE
static void
_tizen_transit_del_cb(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Evas_Coord x, y, w, h;
   Elm_Naviframe_Item *it = data;

   evas_object_geometry_get(WIDGET(it), &x, &y, &w, &h);
   evas_object_move(VIEW(it), x, y);
   evas_object_resize(VIEW(it), w, h);
   evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
   it->tizen_transit = NULL;
}

static void
_tizen_transit_pop_del_cb(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Naviframe_Item *it = data;
   Evas_Coord x, y, w, h;

   evas_object_geometry_get(WIDGET(it), &x, &y, &w, &h);
   evas_object_move(VIEW(it), x, y);
   evas_object_resize(VIEW(it), w, h);
   evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
   it->tizen_transit = NULL;
}

// TIZEN ONLY
static Elm_Transit_Effect *
_custom_context_new(Evas_Object *obj,
                    double from_zoom, double to_zoom,
                    int from_alpha, int to_alpha)
{
   Evas_Coord w, h;
   Evas_Map *map;

   if (!obj) return NULL;

   struct custom_effect *custom_effect = calloc(1, sizeof(struct custom_effect));
   if (!custom_effect) return NULL;

   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   map = evas_map_new(4);

   custom_effect->obj = obj;
   custom_effect->w = w;
   custom_effect->h = h;
   custom_effect->map = map;
   custom_effect->from.zoom = from_zoom;
   custom_effect->from.alpha = from_alpha;
   custom_effect->to.zoom = to_zoom;
   custom_effect->to.alpha = to_alpha;

   return custom_effect;
}

static void
_custom_context_free(Elm_Transit_Effect *effect,
                                 Elm_Transit *transit EINA_UNUSED)
{
   struct custom_effect *custom_effect = effect;
   evas_map_free(custom_effect->map);
   free(custom_effect);
}

static void
_custom_op(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED,
           double progress)
{
   if (!effect) return;

   Evas_Coord w, h;
   Evas_Map *map;
   double zoom;
   int color;

   struct custom_effect *custom_effect = (struct custom_effect *) effect;

   zoom = (custom_effect->from.zoom * (1.0 - progress)) + (custom_effect->to.zoom * progress);
   w = (Evas_Coord) (custom_effect->w * zoom);
   h = (Evas_Coord) (custom_effect->h * zoom);

   map = custom_effect->map;
   evas_map_util_points_populate_from_object_full(map, custom_effect->obj, 0);

   color = (int) ((custom_effect->from.alpha * (1.0 - progress)) +
                  (custom_effect->to.alpha * progress));
   evas_map_point_color_set(map, 0, color, color, color, color);
   evas_map_point_color_set(map, 1, color, color, color, color);
   evas_map_point_color_set(map, 2, color, color, color, color);
   evas_map_point_color_set(map, 3, color, color, color, color);
   evas_map_util_zoom(map, (double) w / custom_effect->w, (double) h / custom_effect->h,
                      custom_effect->w / 2, custom_effect->h / 2);

   evas_map_smooth_set(map, EINA_FALSE);
   evas_object_map_enable_set(custom_effect->obj, EINA_TRUE);
   evas_object_map_set(custom_effect->obj, map);
}

EAPI void
tizen_pop_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit;
   Elm_Transit_Effect *effect_context;

   if (!it->tizen_zoom) return;
   if (it->tizen_transit) elm_transit_del(it->tizen_transit);
   it->tizen_transit = NULL;

   transit = elm_transit_add();
   if (!transit) return;

   it->tizen_transit = transit;

   effect_context = _custom_context_new(VIEW(it), 1.0, 0.0, 255, 255);
   if (!effect_context) return;

   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_add(transit, _custom_op, effect_context, _custom_context_free);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_DECELERATE);
   elm_transit_duration_set(transit, 0.3);
   elm_transit_objects_final_state_keep_set(transit, EINA_TRUE);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_pop_del_cb, it);
   elm_transit_go(transit);
}

EAPI void
tizen_push_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit;
   Elm_Transit_Effect *effect_context;

   if (!it->tizen_zoom) return;

   if (it->tizen_transit) elm_transit_del(it->tizen_transit);
   it->tizen_transit = NULL;

   transit = elm_transit_add();
   if (!transit) return;

   it->tizen_transit = transit;

   effect_context = _custom_context_new(VIEW(it), 0.0, 1.0, 255, 255);
   if (!effect_context) return;

   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_add(transit, _custom_op, effect_context, _custom_context_free);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_DECELERATE);
   elm_transit_duration_set(transit, 0.3);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_del_cb, it);
   elm_transit_go(transit);
}
#else /* mobile */
static void
_tizen_transit_del_cb(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Evas_Coord x, y, w, h;
   Elm_Naviframe_Item *it = data;

   evas_object_geometry_get(WIDGET(it), &x, &y, &w, &h);
   evas_object_move(VIEW(it), x, y);
   evas_object_resize(VIEW(it), w, h);
   evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
   it->tizen_transit = NULL;
}

static void
_tizen_transit_pop_del_cb(void *data, Elm_Transit *transit EINA_UNUSED)
{
   Elm_Naviframe_Item *it = data;
   Evas_Coord x, y, w, h;

   evas_object_geometry_get(WIDGET(it), &x, &y, &w, &h);
   evas_object_move(VIEW(it), x, y);
   evas_object_resize(VIEW(it), w, h);
   evas_object_freeze_events_set(VIEW(it), EINA_FALSE);
   it->tizen_transit = NULL;
}

//Tizen Only
static Elm_Transit_Effect *
_custom_context_new(Evas_Object *obj,
                    double from_zoom, double to_zoom,
                    int from_alpha, int to_alpha)
{
   Evas_Coord w, h;
   Evas_Map *map;

   if (!obj) return NULL;

   struct custom_effect *custom_effect = calloc(1, sizeof(struct custom_effect));
   if (!custom_effect) return NULL;

   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   map = evas_map_new(4);

   custom_effect->obj = obj;
   custom_effect->w = w;
   custom_effect->h = h;
   custom_effect->map = map;
   custom_effect->from.zoom = from_zoom;
   custom_effect->from.alpha = from_alpha;
   custom_effect->to.zoom = to_zoom;
   custom_effect->to.alpha = to_alpha;

   return custom_effect;
}

static void
_custom_context_free(Elm_Transit_Effect *effect,
                                 Elm_Transit *transit EINA_UNUSED)
{
   struct custom_effect *custom_effect = effect;
   evas_map_free(custom_effect->map);
   free(custom_effect);
}

static void
_custom_op(Elm_Transit_Effect *effect, Elm_Transit *transit EINA_UNUSED,
           double progress)
{
   if (!effect) return;

   Evas_Coord w, h;
   Evas_Map *map;
   double zoom;
   int color;

   struct custom_effect *custom_effect = (struct custom_effect *) effect;

   zoom = (custom_effect->from.zoom * (1.0 - progress)) + (custom_effect->to.zoom * progress);
   w = (Evas_Coord) (custom_effect->w * zoom);
   h = (Evas_Coord) (custom_effect->h * zoom);

   map = custom_effect->map;
   evas_map_util_points_populate_from_object_full(map, custom_effect->obj, 0);

   color = (int) ((custom_effect->from.alpha * (1.0 - progress)) +
                  (custom_effect->to.alpha * progress));
   evas_map_point_color_set(map, 0, color, color, color, color);
   evas_map_point_color_set(map, 1, color, color, color, color);
   evas_map_point_color_set(map, 2, color, color, color, color);
   evas_map_point_color_set(map, 3, color, color, color, color);
   evas_map_util_zoom(map, (double) w / custom_effect->w, (double) h / custom_effect->h,
                      custom_effect->w / 2, custom_effect->h / 2);

   evas_map_smooth_set(map, EINA_FALSE);
   evas_object_map_enable_set(custom_effect->obj, EINA_TRUE);
   evas_object_map_set(custom_effect->obj, map);
}

EAPI void
tizen_pop_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit;
   Elm_Transit_Effect *effect_context;

   if (!it->tizen_zoom) return;
   if (it->tizen_transit) elm_transit_del(it->tizen_transit);
   it->tizen_transit = NULL;

   transit = elm_transit_add();
   if (!transit) return;

   it->tizen_transit = transit;

   effect_context = _custom_context_new(VIEW(it), 1.0, 0.8, 255, 0);
   if (!effect_context) return;

   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_add(transit, _custom_op, effect_context, _custom_context_free);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_DECELERATE);
   elm_transit_duration_set(transit, 0.3);
   elm_transit_objects_final_state_keep_set(transit, EINA_TRUE);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_pop_del_cb, it);
   elm_transit_go(transit);
}

EAPI void
tizen_push_effect(Elm_Naviframe_Item *it)
{
   Elm_Transit *transit;
   Elm_Transit_Effect *effect_context;

   if (!it->tizen_zoom) return;

   if (it->tizen_transit) elm_transit_del(it->tizen_transit);
   it->tizen_transit = NULL;

   transit = elm_transit_add();
   if (!transit) return;

   it->tizen_transit = transit;

   effect_context = _custom_context_new(VIEW(it), 0.8, 1.0, 0, 255);
   if (!effect_context) return;

   elm_transit_object_add(transit, VIEW(it));
   elm_transit_effect_add(transit, _custom_op, effect_context, _custom_context_free);
   elm_transit_tween_mode_set(transit, ELM_TRANSIT_TWEEN_MODE_DECELERATE);
   elm_transit_duration_set(transit, 0.3);
   elm_transit_smooth_set(transit, EINA_FALSE);
   elm_transit_del_cb_set(transit, _tizen_transit_del_cb, it);
   elm_transit_go(transit);
}
#endif

EAPI void
obj_hide(Elm_Naviframe_Item *module_data EINA_UNUSED)
{
   return;
}

// module api funcs needed
EAPI int
elm_modapi_init(void *m EINA_UNUSED)
{
   return 1; // succeed always
}

EAPI int
elm_modapi_shutdown(void *m EINA_UNUSED)
{
   return 1; // succeed always
}
