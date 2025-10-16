import fitz
import os


def remove_images(input_pdf, output_pdf):
    doc = fitz.open(input_pdf)
    for page in doc:
        img_list = page.get_images()
        for img in img_list:
            page.delete_image(img[0])

    doc.save(output_pdf)
    doc.close()


def remove_images_from_folder(folder_path):
    # Cria pasta de saída
    output_folder = os.path.join(folder_path, "sem_imagens")
    os.makedirs(output_folder, exist_ok=True)

    # Processa todos os PDFs
    for filename in os.listdir(folder_path):
        if filename.endswith(".pdf"):
            input_path = os.path.join(folder_path, filename)
            output_path = os.path.join(output_folder, filename)

            print(f"Processando: {filename}")
            remove_images(input_path, output_path)
            print(f"Salvo em: {output_path}")


# Caminho da pasta com os PDFs
folder_path = r"C:\Users\gabri\Documents\POLI_Docs\1. Circuitos 2 ⚠️\P2"
remove_images_from_folder(folder_path)
