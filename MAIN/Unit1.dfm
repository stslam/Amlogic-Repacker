object Form1: TForm1
  Left = 198
  Top = 123
  Width = 760
  Height = 455
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 16
  object PG_Ctl: TPageControl
    Left = 0
    Top = 0
    Width = 744
    Height = 381
    ActivePage = ts_Lvl2
    Align = alTop
    TabIndex = 1
    TabOrder = 0
    OnChange = PG_CtlChange
    OnChanging = PG_CtlChanging
    object ts_Lvl1: TTabSheet
      Caption = 'Level_1'
      object Label1: TLabel
        Left = 4
        Top = 8
        Width = 268
        Height = 16
        Caption = 'SOURCE: (UBoot based  firmware file (*.img))'
      end
      object Label2: TLabel
        Left = 8
        Top = 108
        Width = 568
        Height = 16
        Caption = 
          'DESTINATION: (Select the ROOT folder, individual files will be c' +
          'reated in the "Level1" subfolder)'
      end
      object Lbl_DstLegend: TLabel
        Left = 4
        Top = 155
        Width = 481
        Height = 16
        Caption = 
          'Legend: Black - Empty,  Blue - Is NOT Empty, Red - Does not exis' +
          't, will be created'
      end
      object Ed_ImgFile: TEdit
        Left = 8
        Top = 28
        Width = 688
        Height = 24
        TabOrder = 0
        Text = 'D:\MINIX\ZZZ\S912-v008-20180209.img'
      end
      object Btn_FWImgSel: TButton
        Left = 700
        Top = 24
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 1
        OnClick = Btn_FWImgSelClick
      end
      object Btn_UnpLvl1: TButton
        Left = 64
        Top = 320
        Width = 601
        Height = 25
        Caption = 
          'Unpack to level 1 (split to separate files and place it'#39's to the' +
          ' destination subfolder "LEVEL1") '
        TabOrder = 2
        OnClick = Btn_UnpLvl1Click
      end
      object ChkBx_SrcCrc32: TCheckBox
        Left = 0
        Top = 56
        Width = 733
        Height = 17
        Caption = 
          'Do check control sum of the source file (slight slowdown if enab' +
          'led, but disabling this option is strictly NOT recommended)'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
      object Ed_TargetDir: TEdit
        Left = 12
        Top = 128
        Width = 685
        Height = 24
        TabOrder = 4
        Text = 'D:\MINIX\FWWORK'
        OnChange = Ed_TargetDirChange
      end
      object Btn_SelDest: TButton
        Left = 700
        Top = 125
        Width = 25
        Height = 25
        Caption = '...'
        TabOrder = 5
        OnClick = Btn_SelDestClick
      end
      object Btn_ClearDest: TButton
        Left = 8
        Top = 176
        Width = 205
        Height = 25
        Caption = 'Clear all data in the  DEST folder'
        TabOrder = 6
        OnClick = Btn_ClearDestClick
      end
    end
    object ts_Lvl2: TTabSheet
      Caption = 'Level_2'
      ImageIndex = 1
      object LsBx_FilesL1: TListBox
        Left = 0
        Top = 8
        Width = 245
        Height = 324
        IntegralHeight = True
        ItemHeight = 16
        TabOrder = 0
      end
      object Button2: TButton
        Left = 252
        Top = 308
        Width = 121
        Height = 25
        Caption = 'Extract DTB'
        TabOrder = 1
        OnClick = Button2Click
      end
    end
    object ts_Log: TTabSheet
      Caption = 'Log'
      ImageIndex = 2
      object LsBxLog: TListBox
        Left = 0
        Top = 0
        Width = 736
        Height = 340
        Align = alClient
        IntegralHeight = True
        ItemHeight = 16
        TabOrder = 0
      end
    end
  end
  object ChkBx_CleanWarn: TCheckBox
    Left = 291
    Top = 391
    Width = 190
    Height = 17
    Caption = 'Prompt before clean folder'
    Checked = True
    State = cbChecked
    TabOrder = 1
  end
  object OpenDialog: TOpenDialog
    OnShow = OpenDialogShow
    Filter = 'Firmware files *.img|*.img|All files *.*|*.*'
    FilterIndex = 0
    Left = 700
    Top = 12
  end
end
