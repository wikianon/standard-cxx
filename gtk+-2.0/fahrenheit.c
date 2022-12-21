#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *entrada;
GtkWidget *label_saida;

gboolean converter(GtkButton *button, gpointer data)
{
  double celsius;
  double fahrenheit;
  char texto[128];

  celsius=atol(gtk_entry_get_text(GTK_ENTRY(entrada)));

  fahrenheit=celsius*1.8+32;
  sprintf(texto, "%.1f ºC = %.1f ºF", celsius, fahrenheit);
  gtk_label_set_text(GTK_LABEL(label_saida), texto);

  return FALSE;
}

int main(int argc, char **argv)
{
  GtkWidget *janela;
  GtkWidget *conteudo;
  GtkWidget *label_entrada;
  GtkWidget *botao;

  gtk_init(&argc, &argv);

  janela=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(janela), GTK_WIN_POS_CENTER);

  conteudo=gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(janela), conteudo);

  label_entrada=gtk_label_new("Digite a temperatura em ºC");
  gtk_box_pack_start(GTK_BOX(conteudo), label_entrada, FALSE, FALSE, 0);

  entrada=gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(conteudo), entrada, FALSE, FALSE, 0);

  botao=gtk_button_new_with_label("Converter para ºF");
  gtk_box_pack_start(GTK_BOX(conteudo), botao, FALSE, FALSE, 0);

  label_saida=gtk_label_new("ºC = ºF");
  gtk_box_pack_start(GTK_BOX(conteudo), label_saida, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(janela), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(botao), "clicked", G_CALLBACK(converter), NULL);

  gtk_widget_show_all(janela);

  gtk_main();

  return 0;
} 
