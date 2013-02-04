#include <Elementary.h>
#include <tts.h>

/* to enable this module
export ELM_MODULES="access_output_tts>access/api"
export ELM_ACCESS_MODE=1
 */

static tts_h tts;
static Eina_Strbuf *buf = NULL;

static void _text_keep(const char *txt)
{
   if (!buf) return;
   if (eina_strbuf_length_get(buf) > 0) eina_strbuf_append(buf, ", ");
   eina_strbuf_append(buf, txt);
}

static void _text_add()
{
   int ret = 0;
   int u_id = 0;
   char *txt;

   if (!buf) return;
   if (!eina_strbuf_length_get(buf)) return;;

   txt = eina_strbuf_string_steal(buf);
   ret = tts_add_text(tts, txt, NULL, TTS_VOICE_TYPE_AUTO,
                      TTS_SPEED_AUTO, &u_id);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to add kept text : ret(%d)\n", ret);
     }
}

void _tts_state_changed_cb(tts_h tts, tts_state_e previous, tts_state_e current, void* data)
{
   int ret = 0;

   if (TTS_STATE_CREATED == previous && TTS_STATE_READY == current)
     {
        _text_add();

        ret = tts_play(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to play TTS : ret(%d)\n", ret);
          }
     }
}

EAPI int
elm_modapi_init(void *m )
{
   int ret = 0;

   if (tts)
     {
        ret = tts_destroy(tts);
        if (TTS_ERROR_NONE != ret)
          fprintf(stderr, "Fail to destroy handle : result(%d)", ret);
     }

   ret = tts_create(&tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to get handle : result(%d)", ret);
        return ret;
     }

   ret = tts_set_state_changed_cb(tts, _tts_state_changed_cb, NULL);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to set callback : result(%d)", ret);
        return ret;
     }

   ret = tts_set_mode(tts, TTS_MODE_SCREEN_READER);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to set mode : result(%d)", ret);
        return ret;
     }

   ret = tts_prepare(tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to prepare handle : result(%d)", ret);
        return ret;
     }

   buf = eina_strbuf_new();

   return 1;
}

EAPI int
elm_modapi_shutdown(void *m )
{
   int ret = 0;
   if (tts)
     {
        /* check current state */
        tts_state_e state;
        tts_get_state(tts, &state);
        if (state == TTS_STATE_PLAYING || state == TTS_STATE_PAUSED)
          {
             ret = tts_stop(tts);
             if (TTS_ERROR_NONE != ret)
               {
                  fprintf(stderr, "Fail to stop handle : result(%d)", ret);
                  return ret;
               }
          }

        ret = tts_unprepare(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to unprepare handle : result(%d)", ret);
             return ret;
          }

        ret = tts_destroy(tts);
        if (TTS_ERROR_NONE != ret)
          {
             fprintf(stderr, "Fail to destroy handle : result(%d)", ret);
             return ret;
          }

        if (buf)
          {
             eina_strbuf_free(buf);
             buf = NULL;
          }
     }
   return 1;
}

EAPI void
out_read(const char *txt)
{
   int ret = 0;
   int u_id = 0;

   tts_state_e state;
   tts_get_state(tts, &state);
   if (state != TTS_STATE_PLAYING &&
       state != TTS_STATE_PAUSED &&
       state != TTS_STATE_READY)
     {
        if (txt) _text_keep(txt);
        return;
     }

   ret = tts_add_text(tts, txt, NULL, TTS_VOICE_TYPE_AUTO,
                      TTS_SPEED_AUTO, &u_id);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to add text : %s ret(%d)\n", txt, ret);
     }

   /* check current state */
   tts_get_state(tts, &state);
   if (state == TTS_STATE_PLAYING) return;

   ret = tts_play(tts);
   if (TTS_ERROR_NONE != ret)
     {
        fprintf(stderr, "Fail to play TTS : ret(%d)\n", ret);
     }
   return;
}


EAPI void
out_cancel(void)
{
   int ret = 0;
   if (tts)
     {
        /* check current state */
        tts_state_e state;

        tts_get_state(tts, &state);
        if (state != TTS_STATE_PLAYING && state != TTS_STATE_PAUSED)
          {
             if (buf && (eina_strbuf_length_get(buf) > 0))
               eina_strbuf_string_free(buf);
             return;
          }

         ret = tts_stop(tts);
         if (TTS_ERROR_NONE != ret)
           {
             fprintf(stderr, "Fail to stop TTS : result(%d)", ret);
           }
     }
   return;
}

/*
 * unused api
 */

#if 0
EAPI void
out_read_done(void)
{
}

static void (*cb_func) (void *data);
static void *cb_data;

EAPI void
out_done_callback_set(void (*func) (void *data), const void *data)
{
   cb_func = func;
   cb_data = (void *)data;
}
#endif
